package com.playtradex;

import android.app.Activity;

/**
 * Unreal Engine bridge for the PlayTradeX Android file picker.
 *
 * This class keeps the JNI callback boundary inside the stable
 * com.playtradex package instead of depending on Unreal Engine's
 * generated GameActivity Java package.
 */
public final class PlayTradeXFilePickerBridge {
  private PlayTradeXFilePickerBridge() {}

  /**
   * Opens the Android document picker for wallet import.
   *
   * @param activity Current Unreal Engine Activity.
   */
  public static void openImportPicker(Activity activity) {
    android.util.Log.e(
      "PlayTradeXPicker",
      "Bridge openExportPicker"
    );
    if (activity == null) {
      nativeImportResult("");
      return;
    }

    PlayTradeXFilePicker.openImportPicker(
      activity,
      new PlayTradeXFilePicker.Callback() {
        @Override
        public void onResult(String uri) {
          nativeImportResult(
            uri != null ? uri : ""
          );
        }
      }
    );
  }

  /**
   * Opens the Android document picker for wallet export.
   *
   * @param activity Current Unreal Engine Activity.
   */
  public static void openExportPicker(Activity activity) {
    if (activity == null) {
      nativeExportResult("");
      return;
    }

    PlayTradeXFilePicker.openExportPicker(
      activity,
      new PlayTradeXFilePicker.Callback() {
        @Override
        public void onResult(String uri) {
          nativeExportResult(
            uri != null ? uri : ""
          );
        }
      }
    );
  }

  /**
   * Native callback implemented by the PlayTradeX Unreal plugin.
   */
  private static native void nativeImportResult(
    String uri
  );

  /**
   * Native callback implemented by the PlayTradeX Unreal plugin.
   */
  private static native void nativeExportResult(
    String uri
  );
}