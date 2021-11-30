package com.saladmixer.saladmixer;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.google.firebase.FirebaseApp;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;
import java.util.Random;

public class MainActivity extends AppCompatActivity {
    //Firebase files
    public static FirebaseDatabase DATABASE;
    public static DatabaseReference DATAREF;
    public static FirebaseAuth MYAUTH;
    public static FirebaseUser CURRENT_USER;
    public static String USERID;
    public static Salad CURRENT_SALAD;
    public ArrayList<Ingredient> tempIngredientList = new ArrayList<>();

    @Override
    protected void onStart() {
        super.onStart();
        CURRENT_USER = MYAUTH.getCurrentUser();
        updateData(CURRENT_USER);
        if(CURRENT_USER == null){
            Intent loginIntent = new Intent(this,Login.class);
            startActivity(loginIntent);
        }
        updateSaladUI();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //Initialize Firebase
        if (FirebaseApp.getApps(this).size() == 0)
            FirebaseDatabase.getInstance().setPersistenceEnabled(true);
        DATABASE = FirebaseDatabase.getInstance();
        MYAUTH = FirebaseAuth.getInstance();
        CURRENT_USER = MYAUTH.getCurrentUser();
        updateData(CURRENT_USER);

        //Hide salad info display if there is no salad
        updateSaladUI();
    }

    public void addIngredient(View view){
        Intent intent = new Intent(this, addIngredient.class);
        startActivity(intent);

    }

    public void updateData(FirebaseUser user){
        if(user == null){
            USERID = null;
            DATAREF = DATABASE.getReference().child("DEFAULT");
        }
        else{
            USERID = user.getUid();
            DATAREF = DATABASE.getReference().child(USERID);
        }
    }

    public void onClickLogOut(View view){
        MYAUTH.signOut();
        CURRENT_USER = null;
        updateData(null);
        Intent loginIntent = new Intent(this,Login.class);
        startActivity(loginIntent);
    }

    public void onClickBrowseIngredients(View view){
        Intent intentBrowseIngredients = new Intent(this,BrowseIngredients.class);
        startActivity(intentBrowseIngredients);
    }

    public void newSalad(View view){
        DATAREF = DATABASE.getReference().child(USERID).child("ingredients");
        DATAREF.orderByKey().addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                tempIngredientList = new ArrayList<>();
                //Read data from default ingredients
                for(DataSnapshot ingredient: dataSnapshot.getChildren()){
                    tempIngredientList.add(ingredient.getValue(Ingredient.class));
                }
                int index = tempIngredientList.size();
                ArrayList<Integer> indexList = new ArrayList<>();
                if(index >= 3){
                    Random randomGen = new Random();
                    int i = 0;
                    while(i < 3){
                        int randomIndex = randomGen.nextInt(index);
                        if(!indexList.contains(randomIndex)){
                            indexList.add(randomIndex);
                            i++;
                        }
                    }
                    Ingredient ingredient1 = tempIngredientList.get(indexList.get(0));
                    Ingredient ingredient2 = tempIngredientList.get(indexList.get(1));
                    Ingredient ingredient3 = tempIngredientList.get(indexList.get(2));

                    String saladName = ingredient1.getName() + "-" + ingredient2.getName()
                            + "-" + ingredient3.getName();
                    CURRENT_SALAD = new Salad(saladName, ingredient1, ingredient2, ingredient3);
                    updateSaladUI();
                }
                else{
                    CURRENT_SALAD = null;
                    updateSaladUI();
                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });

    }

    private void updateSaladUI(){
        TextView mainImageText = findViewById(R.id.mainImageText);
        Button viewButton = findViewById(R.id.viewButton);
        Button saveButton = findViewById(R.id.saveButton);

        if(CURRENT_SALAD == null){
            mainImageText.setVisibility(View.INVISIBLE);
            viewButton.setVisibility(View.INVISIBLE);
            saveButton.setVisibility(View.INVISIBLE);
        }
        else{
            mainImageText.setVisibility(View.VISIBLE);
            viewButton.setVisibility(View.VISIBLE);
            saveButton.setVisibility(View.VISIBLE);

            String saladName = CURRENT_SALAD.getIngredient1().getName() +"-\n"
                    + CURRENT_SALAD.getIngredient2().getName() +"- \n"
                    + CURRENT_SALAD.getIngredient3().getName();
            mainImageText.setText(saladName);
        }
    }

    public void onClickSaveSalad(View view){
        if(CURRENT_SALAD != null){
            DATAREF = DATABASE.getReference().child(USERID);
            String saladName = CURRENT_SALAD.getName();
            DATAREF.child("salads").child(saladName).setValue(CURRENT_SALAD);
        }
    }

    public void onClickBrowseSalads(View view){
        Intent intentBrowseSalads = new Intent(this,browseSalads.class);
        startActivity(intentBrowseSalads);
    }

    public void onClickView(View view){
        browseSalads.SELECTED_SALAD = CURRENT_SALAD;
        Intent intentViewSalad = new Intent(this,saladInfo.class);
        startActivity(intentViewSalad);
    }
}
