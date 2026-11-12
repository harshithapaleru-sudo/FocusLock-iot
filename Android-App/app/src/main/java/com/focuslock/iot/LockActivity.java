package com.focuslock.iot;

import android.app.Activity;
import android.os.Bundle;
import android.graphics.Color;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

public class LockActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        LinearLayout layout = new LinearLayout(this);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setGravity(Gravity.CENTER);
        layout.setPadding(30, 30, 30, 30);
        layout.setBackgroundColor(Color.rgb(25, 35, 55));

        TextView title = new TextView(this);
        title.setText("FOCUSLOCK");
        title.setTextSize(30);
        title.setTextColor(Color.WHITE);
        title.setGravity(Gravity.CENTER);

        TextView message = new TextView(this);
        message.setText("Study Mode Active!\n\nThis app is restricted.\nPlease return to your studies.");
        message.setTextSize(20);
        message.setTextColor(Color.WHITE);
        message.setGravity(Gravity.CENTER);
        message.setPadding(10, 40, 10, 40);

        Button button = new Button(this);
        button.setText("Return to Study");
        button.setOnClickListener(v -> finish());

        layout.addView(title);
        layout.addView(message);
        layout.addView(button);

        setContentView(layout);
    }
          }
