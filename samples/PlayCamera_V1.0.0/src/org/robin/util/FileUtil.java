package org.robin.util;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import android.graphics.Bitmap;
import android.os.Environment;
import android.util.Log;

public class FileUtil {
	private static final  String TAG = "TFrobin_FileUtil";
	private static final File parentPath = Environment.getExternalStorageDirectory();
	private static   String storagePath = "";
	private static final String DST_FOLDER_NAME = "DCIM/Camera";

	/**初始化保存路径
	 * @return
	 */
	private static String initPath(){
		if(storagePath.equals("")){
			storagePath = parentPath.getAbsolutePath()+"/" + DST_FOLDER_NAME;
			File f = new File(storagePath);
			if(!f.exists()){
				f.mkdir();
			}
		}
		return storagePath;
	}

	/**保存Bitmap到sdcard
	 * @param b
	 */
	public static void saveBitmap(Bitmap b){

		String path = initPath();
		long dataTake = System.currentTimeMillis();
		String jpegName = path + "/" + dataTake +".jpg";
		Log.i(TAG, "saveBitmap:jpegName = " + jpegName);
		try {
			FileOutputStream fout = new FileOutputStream(jpegName);
			BufferedOutputStream bos = new BufferedOutputStream(fout);
			b.compress(Bitmap.CompressFormat.JPEG, 100, bos);
			bos.flush();
			bos.close();
			Log.i(TAG, "saveBitmap OK");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			Log.i(TAG, "saveBitmap Faile");
			e.printStackTrace();
		}

	}
	
	/**
	   * Saves a Bitmap object to disk for analysis.
	   *
	   * @param bitmap The bitmap to save.
	   * @param filename The location to save the bitmap to.
	   */
	  public static void saveBitmap(final Bitmap bitmap, final String filename) {
	    final String root = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + DST_FOLDER_NAME+"Frame";
	    Log.i(TAG, "Saving "+bitmap.getWidth()+"x"+ bitmap.getHeight() +"bitmap to"+root);
	    final File myDir = new File(root);

	    if (!myDir.mkdirs()) {
	    	Log.i(TAG, "Make dir failed");
	    }

	    final String fname = filename;
	    final File file = new File(myDir, fname);
	    if (file.exists()) {
	      file.delete();
	    }
	    try {
	      final FileOutputStream out = new FileOutputStream(file);
	      bitmap.compress(Bitmap.CompressFormat.PNG, 99, out);
	      out.flush();
	      out.close();
	    } catch (final Exception e) {
	    	e.printStackTrace();
	    }
	  }


}
