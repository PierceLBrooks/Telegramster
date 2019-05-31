
// Author: Pierce Brooks

package org.telegramster.ui;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.net.Uri;
import android.util.Log;

import org.telegramster.messenger.BuildConfig;

import java.io.File;
import java.io.FileOutputStream;

public class DoodleActivity extends com.piercelbrooks.sfml.SFMLActivity {
    private static final String TAG = "DOODLE";
    private static final int REQUEST_CODE = 9009;

    private int requestCode;

    public DoodleActivity() {
        super();
        if (getIntent() == null) {
            requestCode = getRequestCode();
        } else {
            requestCode = getIntent().getIntExtra("requestCode", getRequestCode());
        }
        Log.d(TAG, "requestCode = "+requestCode);
    }

    @Override
    protected String getNativeClass() {
        return BuildConfig.SFML_JNI_CLASS;
    }

    @Override
    public void onBackPressed() {
        Log.d(TAG, "onBackPressed");
        if (requestCode == getRequestCode()) {
            File file;
            Bitmap bitmap;
            boolean success = true;
            String path = getPath();
            String[] split = path.split(File.separator);
            path = ""+File.separatorChar;
            for (int i = 0; i != split.length; ++i) {
                path += split[i];
                Log.d(TAG, path);
                if (i == split.length-1) {
                    break;
                }
                try {
                    file = new File(path);
                    if (!file.exists()) {
                        if (!file.mkdirs()) {
                            success = false;
                        }
                    }
                } catch (Exception exception) {
                    exception.printStackTrace();
                    success = false;
                }
                if (!success) {
                    Log.e(TAG, "Bad path!");
                    break;
                }
                path += File.separatorChar;
            }
            if (!success) {
                setResult(RESULT_CANCELED);
                finish();
                return;
            }
            file = null;
            try {
                int width = getWidth();
                int height = getHeight();
                int[][][] pixels = new int[width][height][4];
                getPixels(pixels);
                bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
                for (int x = 0; x != width; ++x) {
                    for (int y = 0; y != height; ++y) {
                        int[] pixel = pixels[x][y];
                        bitmap.setPixel(x, y, Color.argb(pixel[0], pixel[1], pixel[2], pixel[3]));
                    }
                }
                file = new File(path);
                FileOutputStream output = new FileOutputStream(file);
                bitmap.compress(Bitmap.CompressFormat.PNG, 100, output);
            } catch (Exception exception) {
                exception.printStackTrace();
                success = false;
            }
            if (!success) {
                setResult(RESULT_CANCELED);
                finish();
                return;
            }
            setResult(Activity.RESULT_OK, (new Intent()).setData(Uri.fromFile(file)));
            finish();
        }
        setResult(Activity.RESULT_CANCELED);
        finish();
    }

    public static int getRequestCode() {
        return REQUEST_CODE;
    }

    public String getPath() {
        String path = getApplicationInfo().dataDir;
        if (path.length() != 0) {
            if (path.charAt(path.length()-1) != File.separatorChar) {
                path += File.separatorChar;
            }
        } else {
            path += File.separatorChar;
        }
        return path;
    }

    protected int getWidth() {
        return super.getWidth();
    }

    protected int getHeight() {
        return super.getHeight();
    }

    protected void getPixels(int[][][] pixels) {
        super.getPixels(pixels);
    }
}
