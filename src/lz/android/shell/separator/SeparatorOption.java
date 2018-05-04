package lz.android.shell.separator;

import java.io.File;


public class SeparatorOption {

    /**
     * dex 文件的short[]
     */
    public  byte[] dexBuf;
    /**
     * dex文件。
     */
    public File dexFile;

    /**
     * 配置文件。
     */
    public File configFile;

    /**
     * 输出的dex文件。
     */
    public File outDexFile;

    /**
     * 输出的NISL文件。
     */
    public File outNISLFile;

    /**
     * 输出一个C++文件。
     */
    public File outCPFile;
    /**
     * 选取哪一套sdk，默认为0
     */
    public int whichSDK=0;
    /**
     * 输出一个头文件
     */
    public File outHFile;

}
