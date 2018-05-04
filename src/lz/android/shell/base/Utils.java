package lz.android.shell.base;

import org.apache.log4j.Logger;

import java.io.*;

/**
 * Created by buwai on 25/4/1.
 */
public class Utils {
    private final static Logger log = Logger.getLogger(Utils.class);
    /**
     * 是否是Windows操作系统。
     * @return true：是Windows操作系统。false：不是Windows操作系统。
     */
    public static boolean isWindowsOS() {
        String os = System.getProperty("os.name");
        if(os.toLowerCase().startsWith("win")){
            return true;
        } else {
            return false;
        }
    }

    /**
     * 复制单个文件
     *
     * @param oldPath
     *            String 原文件路径 如：c:/fqf.txt
     * @param newPath
     *            String 复制后路径 如：f:/fqf.txt
     * @return boolean
     * @throws IOException
     */
    public static void copyFile(String oldPath, String newPath) throws IOException {
        InputStream inStream = null;
        FileOutputStream fs = null;

        try {
            int bytesum = 0;
            int byteread = 0;
            File oldfile = new File(oldPath);
            if (oldfile.exists()) { // 文件存在时
                inStream = new FileInputStream(oldPath); // 读入原文件
                fs = new FileOutputStream(newPath);
                byte[] buffer = new byte[1444];
                while ((byteread = inStream.read(buffer)) != -1) {
                    bytesum += byteread; // 字节数 文件大小
                    //System.out.println(bytesum);
                    fs.write(buffer, 0, byteread);
                }
            }
        } finally {
            if (null != inStream) {
                inStream.close();
            }
            if (null != fs) {
                fs.close();
            }
        }

    }
    /**
     *递归删除整个文件夹下的所有文件
     *
     */

    public static void forceDelete(File dir)
    {
        if(dir.isFile()) {
            dir.delete();
        }
        else {
            File[] subFileList=dir.listFiles();
            for (File subFile : subFileList) {
                    forceDelete(subFile);
                }
            }
        dir.delete();
        }

    public static byte[] copyByte(byte[] src, int start, int len){
        if(src == null){
            return null;
        }
        if(start > src.length){
            return null;
        }
        if((start+len) > src.length){
            return null;
        }
        if(start<0){
            return null;
        }
        if(len<=0){
            return null;
        }
        byte[] resultByte = new byte[len];
        for(int i=0;i<len;i++){
            resultByte[i] = src[i+start];
        }
        return resultByte;
    }



    /**
     * 复制整个文件夹内容
     *
     * @param oldPath String 原文件路径 如：c:/fqf
     * @param newPath String 复制后路径 如：f:/fqf/ff
     * @return boolean
     */
    public static void copyFolder(String oldPath, String newPath) throws IOException {

        (new File(newPath)).mkdirs(); // 如果文件夹不存在 则建立新文件夹
        File a = new File(oldPath);
        String[] file = a.list();
        File temp = null;

        for (int i = 0; i < file.length; i++) {
            if (oldPath.endsWith(File.separator)) {
                temp = new File(oldPath + file[i]);
            } else {
                temp = new File(oldPath + File.separator + file[i]);
            }

            if (temp.isFile()) {
                try (FileInputStream input = new FileInputStream(temp);
                     FileOutputStream output = new FileOutputStream(newPath
                        + "/" + (temp.getName()).toString())) {

                    byte[] b = new byte[1024 * 5];
                    int len;
                    while ((len = input.read(b)) != -1) {
                        output.write(b, 0, len);
                    }
                    output.flush();
                    output.close();
                    input.close();
                }
            }
            if (temp.isDirectory()) {// 如果是子文件夹
                copyFolder(oldPath + "/" + file[i], newPath + "/" + file[i]);
            }
        }


    }

}
