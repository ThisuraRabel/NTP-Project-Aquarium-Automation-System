package com.example.smartaquamainsys;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.firestore.DocumentReference;
import com.google.firebase.firestore.FirebaseFirestore;

import java.util.HashMap;
import java.util.Map;

public class RegisterPage extends AppCompatActivity {

    public static final String TAG = "TAG";
    Button SubmitBtn, CancelBtn;
    EditText ProductKey,Name, Pass, RePass, PhoneN, EmailR;
    String pKeyCheck = "";
    FirebaseAuth FARDReg;
    ProgressBar pBarReg;
    FirebaseFirestore fS;
    String pID;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register_page);

        SubmitBtn=findViewById(R.id.RegSubmitBtn);
        CancelBtn=findViewById(R.id.RegcancelBtn);

        ProductKey=findViewById(R.id.textProcuctKey);
        Pass= findViewById(R.id.textPassword);
        Name= findViewById(R.id.textName);
        RePass= findViewById(R.id.textRePass);
        PhoneN=findViewById(R.id.textPhoneNumber);
        EmailR= findViewById(R.id.textEmailbox);

        FARDReg = FirebaseAuth.getInstance();
        fS = FirebaseFirestore.getInstance();
        pBarReg = findViewById(R.id.progressBarLogin);



        if(FARDReg.getCurrentUser() != null){
           startActivity(new Intent(getApplicationContext(),MenuPage.class));
           finish();
        }


        SubmitBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                String PKey = ProductKey.getText().toString();
                String NameP = Name.getText().toString();
                String RPass = RePass.getText().toString();
                String Pn = PhoneN.getText().toString();
                String PassWd = Pass.getText().toString().trim();
                final String Email = EmailR.getText().toString().trim();

               // Check Pkey
       
                //Validation checks
                if(TextUtils.isEmpty(Email)){
                    EmailR.setError("Email is Mandatory");
                    return;
                }
                if(TextUtils.isEmpty(PKey)){
                    ProductKey.setError("Product Key is Mandatory");
                    return;
                }
                if(TextUtils.isEmpty(NameP)){
                    Name.setError("Name is Mandatory");
                    return;
                }
                if(TextUtils.isEmpty(Pn)){
                    PhoneN.setError("Phone Number is Mandatory");
                    return;
                }
                if(TextUtils.isEmpty(PassWd)){
                    Pass.setError("Password is Mandatory");
                    return;
                }
                if(TextUtils.isEmpty(RPass)){
                    RePass.setError("Enter password again");
                    return;
                }
                if(PassWd.length() < 6){
                    Pass.setError("Enter 6 or more characters for password");
                    return;
                }
                if(!PassWd.equals(RPass)){
                    Toast.makeText(RegisterPage.this, "Password not matching!", Toast.LENGTH_SHORT).show();
                    return;
                }


               // pBarReg.setVisibility(View.VISIBLE);

                FARDReg.createUserWithEmailAndPassword(Email,PassWd).addOnCompleteListener(new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {
                        if(task.isSuccessful()){
                            Toast.makeText(RegisterPage.this, "Register Successful!", Toast.LENGTH_SHORT).show();

                            pID = FARDReg.getCurrentUser().getUid();
                            DocumentReference documentReference = fS.collection("person").document(pID);
                            Map<String,Object> person = new HashMap<>();
                            person.put("fName", NameP);
                            person.put("pNum", Pn);

                            documentReference.set(person).addOnSuccessListener(new OnSuccessListener<Void>() {
                                @Override
                                public void onSuccess(Void unused) {
                                    Log.d(TAG, "onSuccess: person's Profile created for " + pID);
                                }
                            });

                            startActivity(new Intent(getApplicationContext(),LoginPage.class));
                        }
                        else{
                            Toast.makeText(RegisterPage.this, "Error: " + task.getException().getMessage(), Toast.LENGTH_SHORT).show();
                        }

                    }
                });


                /*
                //check product key mismatch
                if ( PKey == pKeyCheck ){

                    //if send sucsess
                    Intent i = new Intent(getApplicationContext(), LoginPage.class);
                    startActivity(i);

                    //else
                    //display toast "Data Send Error!"
                }
                else{
                    //display toast "Product Key Not valid!"
                }
                */


            }
        });

/*
        CancelBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // go to  connect page
            }
        });
*/

    }
}