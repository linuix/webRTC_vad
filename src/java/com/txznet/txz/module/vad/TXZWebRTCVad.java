package com.txznet.txz.module.vad;

import java.io.File;
import java.lang.String;

import javax.xml.crypto.Data;

public class TXZWebRTCVad{


    static{
        System.loadLibrary("txz_webrtcvad");
    }

    public static void main(String[] args) {
        TXZWebRTCVad txzWebRTCVad = new TXZWebRTCVad();
        txzWebRTCVad.splitPCM("/home/link/workspaces/speech-vad-demo/pcm/processaudio.pcm", "/home/link/workspaces/speech-vad-demo/output_pcm");
    }

    public native void splitPCM(String pcmPath,String outPath);

}