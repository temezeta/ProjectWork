package com.saladmixer.saladmixer;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.os.Bundle;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;

public class BrowseIngredients extends AppCompatActivity {
    private ArrayList<Ingredient> TEMP_INGREDIENTS;
    private customAdapter ADAPTER;
    RecyclerView recyclerView;
    public static Ingredient SELECTED_INGREDIENT;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_browse_ingredients);

        recyclerView = findViewById(R.id.recyclerView);
        ADAPTER = new customAdapter(this);
        recyclerView.setAdapter(ADAPTER);
        recyclerView.setLayoutManager(new LinearLayoutManager(this));
        SELECTED_INGREDIENT = null;

        MainActivity.DATAREF = MainActivity.DATABASE.getReference().child(MainActivity.USERID);
        TEMP_INGREDIENTS = new ArrayList<>();

        MainActivity.DATAREF.orderByKey().addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                //Read data from user
                TEMP_INGREDIENTS = new ArrayList<>();
                for(DataSnapshot ingredient: dataSnapshot.child("ingredients").getChildren()){
                    TEMP_INGREDIENTS.add(ingredient.getValue(Ingredient.class));
                }
                ADAPTER.setIngredients(TEMP_INGREDIENTS);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });
    }
}
