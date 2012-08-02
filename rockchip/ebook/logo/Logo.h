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

#ifndef RK_LOGO_H
#define RK_LOGO_H

#include <stdint.h>
#include <sys/types.h>

#include <utils/threads.h>
#include <utils/AssetManager.h>

#include <surfaceflinger/ISurfaceComposer.h>
#include <surfaceflinger/SurfaceComposerClient.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

class SkBitmap;

namespace android {

class AssetManager;

// ---------------------------------------------------------------------------

class Logo : public Thread, public IBinder::DeathRecipient
{
public:
                Logo();
    virtual     ~Logo();

    sp<SurfaceComposerClient> session() const;

private:
    virtual bool        threadLoop();
    virtual status_t    readyToRun();
    virtual void        onFirstRef();
    virtual void        binderDied(const wp<IBinder>& who);

    struct Texture {
        GLint   w;
        GLint   h;
        GLuint  name;
    };

    status_t initTexture(Texture* texture);
    bool show_logo();

    sp<SurfaceComposerClient>       mSession;
    AssetManager mAssets;
    Texture     mLogo;
    int         mWidth;
    int         mHeight;
    EGLDisplay  mDisplay;
    EGLDisplay  mContext;
    EGLDisplay  mSurface;
    sp<SurfaceControl> mFlingerSurfaceControl;
    sp<Surface> mFlingerSurface;
};

// ---------------------------------------------------------------------------

}; // namespace android

#endif // RK_LOGO_H
