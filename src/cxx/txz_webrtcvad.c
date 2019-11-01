#include "jni.h"
#include <stdio.h>
#include <stdlib.h>
#include "log_util.h"
#include"simple_vad.h"
#include <common_audio/vad/include/webrtc_vad.h>
#include "period_format.h"
#include "file_cut.h"
#include <unistd.h>

void splitPCM(JNIEnv *env, jobject obj, jstring jpcmPath, jstring joutPath);
int run(FILE *fp, simple_vad *vad, struct cut_info *cut);

int add_period_activity(struct periods *per, int is_active, int is_last);
void splitPCM_pro( const char *pcmPath, const char *outPath);

static JNINativeMethod gMethods[] = {
        {"splitPCM","(Ljava/lang/String;Ljava/lang/String;)V" ,(void*)splitPCM}
};

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* vm, void* reserved){
    LOGD("vad-project JNI_OnLoad");
    jint result = -1;
    JNIEnv* gEnv;
    if((*vm)->GetEnv((vm),(void**)&gEnv, JNI_VERSION_1_6) != JNI_OK){
        return JNI_ERR;
    }
    jclass cls = (*gEnv)->FindClass((gEnv),"com/txznet/txz/module/vad/TXZWebRTCVad");
    if (NULL != cls)
    {
        (*gEnv)->RegisterNatives(gEnv, cls, gMethods,sizeof(gMethods)/sizeof((gMethods)[0]));
    }
    
    return JNI_VERSION_1_6;
}

/*
 * Class:     com_txznet_txz_module_vad_TXZWebRTCVad
 * Method:    splitPCM
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */

void splitPCM(JNIEnv *env, jobject obj, jstring jpcmPath, jstring joutPath)
{
        //   logd("%s\n",__fun__);
    const char *pcmPath = (*env)->GetStringUTFChars(env,jpcmPath,0);
    const char *outPath = (*env)->GetStringUTFChars(env,joutPath,0);
    LOGD("pcmPath = %s, outPath = %s",pcmPath,outPath);
    
    splitPCM_pro(pcmPath,outPath);
}

void splitPCM_pro( const char *pcmPath, const char *outPath){
    LOGD("entrey !");
    FILE *fp = fopen(pcmPath, "rb");
    
    if (fp == NULL) {
        fprintf(stderr, "%s does not exist\n", pcmPath);
        return;
    }
    simple_vad *vad = simple_vad_create();
    if (vad == NULL) {
        return ;
    }
    LOGD("create vad success!");
    FILE *fp2 = fopen(pcmPath, "rb");
    struct cut_info *cut = cut_info_create(fp2);
    snprintf(cut->output_filename_prefix, sizeof(cut->output_filename_prefix), "%s", "output_filename_prefix");
    snprintf(cut->output_file_dir, sizeof(cut->output_file_dir), "%s", outPath);
    int res = run(fp, vad, cut);

    fclose(fp);
    fclose(fp2);
    simple_vad_free(vad);
    cut_info_free(cut);
    printf("PROGRAM FINISH\n");
}



int run(FILE *fp, simple_vad *vad, struct cut_info *cut) {

    int16_t data[FRAME_SIZE];
    int res = 0;
    struct periods *per = periods_create();

    while (res == 0) {
        res = read_int16_bytes(fp, data);
        if (res <= 1) {
            int is_last = (res == 1);
            int is_active = process_vad(vad, data);
            add_period_activity(per, is_active, is_last);
            int vad_file_res = cut_add_vad_activity(cut, is_active, is_last);
            if (vad_file_res < 0) {
                printf("file write success %s\n", cut->result_filename);
            }
        } else if (ferror(fp)) {
            printf("read failed  ferror result  : %d\n", ferror(fp));
        }

    }
    periods_free(per);

    if (res != 1) {
        fprintf(stderr, "read file error %d\n", res);
        return res;
    }
    return 0;
}


int add_period_activity(struct periods *per, int is_active, int is_last) {
    static int old_is_active = 0;
    static int count = 0;
    int res_add = period_add_vad_activity(per, is_active, is_last);
    if (res_add != 0) {
        return res_add;
    }
    if (is_active != old_is_active) {
        // printf("%s,%d \n", old_is_active ? "A" : "I", count);
        // I,1  表示之前的1个FRAME是 INACTIVE的；
        // I,1 A,10 表示之前的1个FRAME是 INACTIVE的；第2-10个FRAME是ACTIVE的
        // periods_print(per);
        old_is_active = is_active;
    }
    count += 1;
    if (is_last) {
        periods_print(per);
        printf("total frames %d\n", count);
    }
}

