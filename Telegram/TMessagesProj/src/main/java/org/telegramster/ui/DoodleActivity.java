
// Author: Pierce Brooks

package org.telegramster.ui;

import org.telegramster.messenger.BuildConfig;

public class DoodleActivity extends com.piercelbrooks.sfml.SFMLActivity {
    public DoodleActivity() {
        super();
    }

    @Override
    protected String getNativeClass() {
        return BuildConfig.SFML_JNI_CLASS;
    }
}
