/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "Logo"

#include <hardware_legacy/power.h>

#include <stdint.h>
#include <sys/types.h>
#include <math.h>
#include <fcntl.h>
#include <utils/misc.h>
#include <signal.h>

#include <binder/IPCThreadState.h>
#include <utils/threads.h>
#include <utils/Atomic.h>
#include <utils/Errors.h>
#include <utils/Log.h>
#include <utils/AssetManager.h>

#include <ui/PixelFormat.h>
#include <ui/Rect.h>
#include <ui/Region.h>
#include <ui/DisplayInfo.h>
#include <ui/FramebufferNativeWindow.h>
#include <ui/EGLUtils.h>

#include <surfaceflinger/ISurfaceComposer.h>
#include <surfaceflinger/ISurfaceFlingerClient.h>

#include <core/SkBitmap.h>
#include <images/SkImageDecoder.h>

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <EGL/eglext.h>

#include "Logo.h"

#define LOGO_PATH "/data/misc/standby-logo.png"
#define DEFAULT_LOGO_PATH "images/standby-logo-static.png"

namespace android {

// ---------------------------------------------------------------------------

Logo::Logo() : Thread(false)
{
    mSession = new SurfaceComposerClient();
}

Logo::~Logo() {
}

void Logo::onFirstRef() {
    status_t err = mSession->linkToComposerDeath(this);
    LOGE_IF(err, "linkToComposerDeath failed (%s) ", strerror(-err));
    if (err == NO_ERROR) {
        run("Logo", PRIORITY_DISPLAY);
    }
}

sp<SurfaceComposerClient> Logo::session() const {
    return mSession;
}


void Logo::binderDied(const wp<IBinder>& who)
{
    // woah, surfaceflinger died!
    LOGD("SurfaceFlinger died, exiting...");

    // calling requestExit() is not enough here because the Surface code
    // might be blocked on a condition variable that will never be updated.
    kill( getpid(), SIGKILL );
    requestExit();
}

status_t Logo::initTexture(Texture* texture) {
    SkBitmap bitmap;
    if(!SkImageDecoder::DecodeFile(LOGO_PATH, &bitmap)) {
	    Asset* asset = mAssets.open(DEFAULT_LOGO_PATH, Asset::ACCESS_BUFFER);
	    if (!asset)
	        return NO_INIT;
	    SkImageDecoder::DecodeMemory(asset->getBuffer(false), asset->getLength(),
	            &bitmap, SkBitmap::kNo_Config, SkImageDecoder::kDecodePixels_Mode);
	    asset->close();
	    delete asset;
    }

    // ensure we can call getPixels(). No need to call unlock, since the
    // bitmap will go out of scope when we return from this method.
    bitmap.lockPixels();

    const int w = bitmap.width();
    const int h = bitmap.height();
    const void* p = bitmap.getPixels();

    GLint crop[4] = { 0, h, w, -h };
    texture->w = w;
    texture->h = h;

    glGenTextures(1, &texture->name);
    glBindTexture(GL_TEXTURE_2D, texture->name);

    switch (bitmap.getConfig()) {
        case SkBitmap::kA8_Config:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA,
                    GL_UNSIGNED_BYTE, p);
            break;
        case SkBitmap::kARGB_4444_Config:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                    GL_UNSIGNED_SHORT_4_4_4_4, p);
            break;
        case SkBitmap::kARGB_8888_Config:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                    GL_UNSIGNED_BYTE, p);
            break;
        case SkBitmap::kRGB_565_Config:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
                    GL_UNSIGNED_SHORT_5_6_5, p);
            break;
        default:
            break;
    }

    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, crop);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    return NO_ERROR;
}

status_t Logo::readyToRun() {
    mAssets.addDefaultAssets();

    DisplayInfo dinfo;
    status_t status = session()->getDisplayInfo(0, &dinfo);
    if (status)
        return -1;

    // create the native surface
    sp<SurfaceControl> control = session()->createSurface(
            getpid(), 0, dinfo.w, dinfo.h, PIXEL_FORMAT_RGB_565);
    session()->openTransaction();
    control->setLayer(0x40000000);
    session()->closeTransaction();

    sp<Surface> s = control->getSurface();

    // initialize opengl and egl
    const EGLint attribs[] = {
            EGL_DEPTH_SIZE, 0,
            EGL_NONE
    };
    EGLint w, h, dummy;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);
    EGLUtils::selectConfigForNativeWindow(display, attribs, s.get(), &config);
    surface = eglCreateWindowSurface(display, config, s.get(), NULL);
    context = eglCreateContext(display, config, NULL, NULL);
    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
        return NO_INIT;

    mDisplay = display;
    mContext = context;
    mSurface = surface;
    mWidth = w;
    mHeight = h;
    mFlingerSurfaceControl = control;
    mFlingerSurface = s;

    return NO_ERROR;
}

bool Logo::threadLoop()
{
    bool r = show_logo();

    eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(mDisplay, mContext);
    eglDestroySurface(mDisplay, mSurface);
    mFlingerSurface.clear();
    mFlingerSurfaceControl.clear();
    eglTerminate(mDisplay);
    IPCThreadState::self()->stopProcess();
    return r;
}

bool Logo::show_logo()
{
	initTexture(&mLogo);

	// Blend state
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mLogo.name);
	glDrawTexiOES(0, 0, 0, mWidth, mHeight); //draw the background image
	eglSwapBuffers(mDisplay, mSurface);

    usleep(500000);
	release_wake_lock("show_logo_lock");
         
	while(1)
	{
		usleep(100000); //delay 100ms
		if(exitPending())
		{
			usleep(1000000); //delay 1s
			break;
		}
	}
	
	glDeleteTextures(1, &mLogo.name);

	return false;
}

// ---------------------------------------------------------------------------

}
; // namespace android
