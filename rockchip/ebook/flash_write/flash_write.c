#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <./linux/fb.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>



long getlength( FILE *fp )
{
 long cur_pos;
 long len;
 
 //取得当前文件流的读取位置
 cur_pos = ftell( fp );
 //将文件流的读取位置设为文件末尾
 fseek( fp, 0, SEEK_END );
 //获取文件末尾的读取位置,即文件大小
 len = ftell( fp );
 //将文件流的读取位置还原为原先的值
 fseek( fp, cur_pos, SEEK_SET );
 return len;
}


int main(int argc, char * argv[])
{
    int i=0;
    int spi=-1;
    FILE *fi = NULL;
    int ret = 0;
    char *buf, *buf1;
    char szName[128];

    int fileSize=0;
    
    spi = open ("/dev/spi_flash", O_RDWR);	//-0
    if (spi < 0)
    {
        printf("can not open spi_flash.\r\n");
        return 1;
    }
  //  printf("fb = 0x%x\n", spi);
   // printf("input file name is %s \n",argv[1]);

    if(argv[1])
    {
        sprintf(szName, "%s", (char*)argv[1]);
    }
    else
    {
        printf("please input file name like \"%s waveform.bin\" \n",(char*)argv[0]);        
        goto end2;
    }
    
    printf("file name is %s \n",szName);
    
    //------open wave form file------ 
    fi = fopen(szName, "r");
    if(fi == NULL)
    {
        printf("%s is not in this directory\n",szName);
        goto end2;
    }
    fileSize = getlength(fi);
    if(fileSize == 0)
    {
        printf("%s is empty!\n",szName);
        goto end2;
    }
    buf = (char*)malloc(fileSize);
    buf1 = (char*)malloc(fileSize);
    
    int nRead = fread(buf, 1, fileSize, fi);	
	
	//printf("nRead = %d\n", nRead);
    //-----------------------------  
    
    ret = write(spi, buf, fileSize);
    
    if (ret < 0)
    {
        printf("can not write spi_flash \n");
        goto end1;
    }
    else
    {
        //printf("spi flash write data 0x%x\n",ret);
    }
   
    ret = read(spi, buf1, fileSize);
   
    while(i<fileSize)
    {
        if(buf1[i] != buf[i])
        {
            printf("spi flash write failed in %d\n",i );
            break;
        }
        i++;
    }

    printf("spi flash write wave form succed !!!\n");

end1:  
    if(buf)free(buf);
    if(buf1)free(buf1);
end2:    
    if(spi)close(spi);
    if(fi)fclose(fi);
    return 0;

}

