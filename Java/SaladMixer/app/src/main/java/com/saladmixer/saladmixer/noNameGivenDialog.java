package com.saladmixer.saladmixer;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;

import androidx.appcompat.app.AppCompatDialogFragment;

public class noNameGivenDialog extends AppCompatDialogFragment {
    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity(),R.style.alertDialog);
        String alertMessage;
        String alertTitle = getResources().getString(R.string.alert_dialog_title);
        String TAG = getTag();
        alertMessage = TAG;
        if(TAG == "Alert Dialog"){
            alertMessage = getResources().getString(R.string.alert_message_no_name);
        }
        else if (TAG == "Wrong Weight"){
            alertMessage = getResources().getString(R.string.alert_message_wrong_weight);
        }
        else if(TAG == "Add Success") {
            alertMessage = getResources().getString(R.string.alert_message_add_success);
            alertTitle = "";
        }
        else if(TAG == "Wrong Data"){
            alertMessage = getResources().getString(R.string.alert_message_wrong_fat) + "\n\n" +
                    getResources().getString(R.string.alert_message_wrong_sugars)+ "\n\n"+
                    getResources().getString(R.string.alert_message_total_weight);
        }
        else if(TAG == "Forgot Password"){
            alertMessage = getResources().getString(R.string.alert_message_forgot_password);
            alertTitle = "";
        }
        else if(TAG == "Defaults Imported"){
            alertMessage = getResources().getString(R.string.alert_message_defaults_imported);
            alertTitle = "";
        }

        builder.setTitle(alertTitle).setMessage(alertMessage).setPositiveButton("OK", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {

            }
        });
        return builder.create();
    }
}
