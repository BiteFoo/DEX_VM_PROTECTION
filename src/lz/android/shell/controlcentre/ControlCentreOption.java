package lz.android.shell.controlcentre;

import java.io.File;

/**
 * Created by Neptunian on 2015/4/3.
 */
public class ControlCentreOption {
    /**
     * dex 文件的short[]
     */
     public  byte[] dexBuf;
    /**
     * apk原文件目录。
     */
    public File apkFile;

    /**
     * 加固后apk的输出目录。
     */
    public File outDir;

    /**
     * 工作目录。
     */
    public File workspace;

    /**
     * 抽离器生成的yc文件。
     */
    public File outYcFile;

    /**
     * 抽离器生成的yc的cpp文件。
     */
    public File outYcCPFile;

    /**
     * 在工作目录中的jni文件存放目录。
     */
    public File jniDir;

    /**
     * lib目录。
     */
    public File libDir;
    /**
     * 抽离生成的yc的h文件
     */
    public File outYchFile;

}
