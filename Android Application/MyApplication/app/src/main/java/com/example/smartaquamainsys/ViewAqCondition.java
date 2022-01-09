package com.example.smartaquamainsys;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.EditText;
import android.widget.TextView;

public class ViewAqCondition extends AppCompatActivity {

    private TextView WLevel;
    private TextView PH;
    private TextView LI;
    private TextView Temp;
    private TextView WorkIndicator;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_view_aq_condition);

        WLevel= (EditText) findViewById(R.id.textEmail);
        PH= (EditText) findViewById(R.id.textFPass);
        LI= (EditText) findViewById(R.id.textEmail);
        Temp= (EditText) findViewById(R.id.textFPass);
        WorkIndicator= (EditText) findViewById(R.id.textFPass);

        //get realtime data from firebase and display


    }
}