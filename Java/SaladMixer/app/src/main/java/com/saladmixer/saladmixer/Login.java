package com.saladmixer.saladmixer;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.EditText;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseUser;

import java.util.regex.Pattern;

public class Login extends AppCompatActivity {
    private EditText editEmail;
    private EditText editPassword;

    private static Pattern PASSWORD_PATTERN =
            Pattern.compile("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[A-Za-z\\d\\W\\_]{8,}$");

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        editEmail = findViewById(R.id.editEmail);
        editPassword = findViewById(R.id.editPassword);
        MainActivity.CURRENT_SALAD = null;

    }

    @Override
    public void onBackPressed() {
        //Disable back button
    }

    public void onClickLogin(View view){
        String email = editEmail.getText().toString();
        String password = editPassword.getText().toString();
        if(isAValidPassword(password) && email.length() != 0){
            MainActivity.MYAUTH.signInWithEmailAndPassword(email,password).addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                @Override
                public void onComplete(@NonNull Task<AuthResult> task) {
                    if (task.isSuccessful()) {
                        FirebaseUser user = MainActivity.MYAUTH.getCurrentUser();
                        MainActivity.CURRENT_USER = user;
                        clearFields();
                        finish();
                    } else {
                        MainActivity.CURRENT_USER = null;
                        openDialog(getResources().getString(R.string.alert_message_login_error));
                    }
                }
            });
        }
        else{
            if(email.length() == 0){
                openDialog(getResources().getString(R.string.alert_message_email_length));
            }
            else if(!isAValidPassword(password)){
                openDialog(getResources().getString(R.string.alert_message_wrong_password_format));
            }
        }

    }

    public void onClickRegister(View view){
        String email = editEmail.getText().toString();
        String password = editPassword.getText().toString();
        if(isAValidPassword(password) && email.length() != 0){
            MainActivity.MYAUTH.createUserWithEmailAndPassword(email, password)
                    .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                        @Override
                        public void onComplete(@NonNull Task<AuthResult> task) {
                            if (task.isSuccessful()) {
                                FirebaseUser user = MainActivity.MYAUTH.getCurrentUser();
                                MainActivity.CURRENT_USER = user;
                                clearFields();
                                finish();
                            } else {
                                MainActivity.CURRENT_USER = null;
                                openDialog(getResources().getString(
                                        R.string.alert_message_register_error));
                            }
                        }
                    });
        }
        else{
            if(email.length() == 0){
                openDialog(getResources().getString(R.string.alert_message_email_length));
            }
            else if(!isAValidPassword(password)){
                openDialog(getResources().getString(R.string.alert_message_wrong_password_format));
            }
        }

    }

    public void onClickForgotPassword(View view){
        String email = editEmail.getText().toString();
        if(email.length() != 0){
            MainActivity.MYAUTH.sendPasswordResetEmail(email)
                    .addOnCompleteListener(new OnCompleteListener<Void>() {
                        @Override
                        public void onComplete(@NonNull Task<Void> task) {
                            if (task.isSuccessful()) {
                            }
                            else{

                            }
                        }
                    });
            openDialog("Forgot Password");
        }
    }

    private boolean isAValidPassword(String password){
        return PASSWORD_PATTERN.matcher(password).matches();
    }

    private void openDialog(String tag){
        noNameGivenDialog dialog = new noNameGivenDialog();
        dialog.show(getSupportFragmentManager(),tag);
    }

    private void clearFields(){
        editEmail.setText("");
        editPassword.setText("");
    }
}
