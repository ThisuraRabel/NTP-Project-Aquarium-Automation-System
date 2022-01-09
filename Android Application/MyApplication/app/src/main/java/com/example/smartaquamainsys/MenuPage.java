package com.example.smartaquamainsys;


import android.content.Intent;
import android.os.Bundle;


import android.view.View;
import android.widget.Button;

import androidx.appcompat.app.AppCompatActivity;

import com.google.firebase.auth.FirebaseAuth;

public class MenuPage extends AppCompatActivity {

    private Button AqCondition, AqConditionReport, ChangeSettings, ManControl, ChangeDetails, Instructions, Logout;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu_page);

        AqCondition=(Button) findViewById(R.id.btn1);
        AqConditionReport=(Button) findViewById(R.id.btn2);
        ChangeSettings=(Button) findViewById(R.id.btn3);
        ManControl=(Button) findViewById(R.id.btn4);
        ChangeDetails=(Button) findViewById(R.id.btn5);
        Instructions=(Button) findViewById(R.id.btn6);
        Logout=(Button) findViewById(R.id.btn7);

        AqCondition.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = new Intent(getApplicationContext(), ViewAqCondition.class);
                startActivity(i);
            }
        });

        AqConditionReport.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = new Intent(getApplicationContext(), AqConditionRep.class);
                startActivity(i);
            }
        });

        ChangeSettings.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = new Intent(getApplicationContext(), ChangeDeviceSettings.class);
                startActivity(i);
            }
        });

        ManControl.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = new Intent(getApplicationContext(), ManControl.class);
                startActivity(i);
            }
        });

        ChangeDetails.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = new Intent(getApplicationContext(), ChangeDetails.class);
                startActivity(i);
            }
        });

        Instructions.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = new Intent(getApplicationContext(), Instructions.class);
                startActivity(i);
            }
        });

        Logout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                FirebaseAuth.getInstance().signOut();
                Intent i = new Intent(getApplicationContext(), LoginPage.class);
                startActivity(i);
                //finish();
            }
        });

    }
}
