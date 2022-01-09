package com.example.smartaquamainsys;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;

public class LoginPage extends AppCompatActivity {

    private Button LoginBtn, RegisterBtn;
    private EditText Email, Pass;
    private FirebaseAuth FARD;
    private ProgressBar pBar;


    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login_page);

        LoginBtn= findViewById(R.id.loginBtn);
        RegisterBtn= findViewById(R.id.RgBtn);
        Email= findViewById(R.id.textEmail);
        Pass= findViewById(R.id.textFPass);
        FARD = FirebaseAuth.getInstance();
        pBar = findViewById(R.id.progressBarLogin);

        LoginBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                String Pwd = Pass.getText().toString().trim();
                String EMail = Email.getText().toString().trim();


                //ManLightOff process1 = new ManLightOff();
                //process1.execute();


                if ( TextUtils.isEmpty(EMail) || TextUtils.isEmpty(Pwd) ){

                    Toast.makeText(LoginPage.this, "Username or password missing!", Toast.LENGTH_SHORT).show();
                    return;
                }
                pBar.setVisibility(View.VISIBLE);

                FARD.signInWithEmailAndPassword(EMail,Pwd).addOnCompleteListener(new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {
                        if(task.isSuccessful()){
                            Toast.makeText(LoginPage.this, "Login Successful!", Toast.LENGTH_SHORT).show();
                            startActivity(new Intent(getApplicationContext(),MenuPage.class));
                        }
                        else{
                            Toast.makeText(LoginPage.this, "Error: " + task.getException().getMessage(), Toast.LENGTH_SHORT).show();
                            pBar.setVisibility(View.GONE);
                        }

                    }
                });



            }
        });


        RegisterBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = new Intent(getApplicationContext(), RegisterPage.class);
                startActivity(i);
            }
        });


    }
}