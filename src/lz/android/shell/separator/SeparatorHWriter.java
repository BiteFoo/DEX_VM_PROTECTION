package lz.android.shell.separator;


import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class SeparatorHWriter {
    private int whichSDK;
    private File mOutFile;
    SeparatorHWriter(int whichSDK,File outFile)
    {
        this.whichSDK=whichSDK;
        this.mOutFile=outFile;
    }

    public void write() throws IOException {
        try (BufferedWriter fileWriter = new BufferedWriter(new FileWriter(mOutFile))) {
            int test =whichSDK;
            fileWriter.write("#define SDKVERSION  ");
            fileWriter.write(Integer.toString(whichSDK));
            fileWriter.newLine();
        }
    }

}
