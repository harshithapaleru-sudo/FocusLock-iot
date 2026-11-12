package com.focuslock.iot;

import android.app.Activity;
import android.os.Bundle;
import android.content.Intent;
import android.provider.Settings;
import android.app.AppOpsManager;
import android.content.Context;
import android.view.Gravity;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.graphics.Color;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        LinearLayout layout = new LinearLayout(this);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setGravity(Gravity.CENTER);
        layout.setPadding(25, 25, 25, 25);
        layout.setBackgroundColor(Color.rgb(20, 30, 50));

        TextView title = new TextView(this);
        title.setText("FOCUSLOCK IoT");
        title.setTextSize(28);
        title.setTextColor(Color.WHITE);
        title.setGravity(Gravity.CENTER);

        TextView info = new TextView(this);
        info.setText("Smart Mobile Usage Restriction\n\nEnable permissions and start study mode.");
        info.setTextSize(17);
        info.setTextColor(Color.WHITE);
        info.setGravity(Gravity.CENTER);

        Button usage = new Button(this);
        usage.setText("Enable Usage Access");
        usage.setOnClickListener(v ->
            startActivity(new Intent(Settings.ACTION_USAGE_ACCESS_SETTINGS))
        );

        Button overlay = new Button(this);
        overlay.setText("Enable Display Over Other Apps");
        overlay.setOnClickListener(v ->
            startActivity(new Intent(
                Settings.ACTION_MANAGE_OVERLAY_PERMISSION,
                android.net.Uri.parse("package:" + getPackageName())
            ))
        );

        Button start = new Button(this);
        start.setText("START STUDY MODE");
        start.setOnClickListener(v -> {
            if (hasUsageAccess() && Settings.canDrawOverlays(this)) {
                startService(new Intent(this, MonitorService.class));
            } else {
                info.setText("Please enable both permissions first.");
            }
        });

        layout.addView(title);
        layout.addView(info);
        layout.addView(usage);
        layout.addView(overlay);
        layout.addView(start);

        setContentView(layout);
    }

    private boolean hasUsageAccess() {
        AppOpsManager appOps =
            (AppOpsManager) getSystemService(Context.APP_OPS_SERVICE);

        int mode = appOps.checkOpNoThrow(
            AppOpsManager.OPSTR_GET_USAGE_STATS,
            android.os.Process.myUid(),
            getPackageName()
        );

        return mode == AppOpsManager.MODE_ALLOWED;
    }
                             }
