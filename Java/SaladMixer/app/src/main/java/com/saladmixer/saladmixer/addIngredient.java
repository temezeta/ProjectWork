package com.saladmixer.saladmixer;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.EditText;

import java.util.ArrayList;

public class addIngredient extends AppCompatActivity {
    private EditText nameField;
    private EditText weightField;
    private EditText caloriesField;
    private EditText fatField;
    private EditText fatSaturatedField;
    private EditText carbohydratesField;
    private EditText carboSugarField;
    private EditText fiberField;
    private EditText proteinField;
    private EditText saltField;
    public ArrayList<Ingredient> tempIngredientList = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_ingredient);
        nameField = findViewById(R.id.editName);
        weightField = findViewById(R.id.editWeight);
        caloriesField = findViewById(R.id.editEnergy);
        fatField = findViewById(R.id.editFat);
        fatSaturatedField = findViewById(R.id.editFatSaturated);
        carbohydratesField = findViewById(R.id.editCarbohydrates);
        carboSugarField = findViewById(R.id.editCarboSugars);
        fiberField = findViewById(R.id.editFiber);
        proteinField = findViewById(R.id.editProtein);
        saltField = findViewById(R.id.editSalt);
    }

    public void onClickaddIngredient(View view){
        String name = nameField.getText().toString().trim();
        String weightValue = weightField.getText().toString();
        String caloriesValue = caloriesField.getText().toString();
        String fatValue = fatField.getText().toString();
        String fatSaturatedValue = fatSaturatedField.getText().toString();
        String carbohydratesValue = carbohydratesField.getText().toString();
        String carboSugarsValue = carboSugarField.getText().toString();
        String fiberValue = fiberField.getText().toString();
        String proteinValue = proteinField.getText().toString();
        String saltValue = saltField.getText().toString();

        //Check for empty name and weight fields
        if(name.matches("") || weightValue.matches("")){
            openDialog("Alert Dialog");
        }
        else {
            double weight = Double.parseDouble(weightValue);
            //Check for 0 weight
            if (weight == 0.0) {
                openDialog("Wrong Weight");
            }
            else{
                double weightFactor = 100.0 / weight;
                //Normalize all ingredients to 100 grams
                double calories = emptyToZero(caloriesValue) * weightFactor;
                double fat = emptyToZero(fatValue) * weightFactor;
                double fatSaturated = emptyToZero(fatSaturatedValue) * weightFactor;
                double carbohydrates = emptyToZero(carbohydratesValue) * weightFactor;
                double carboSugars = emptyToZero(carboSugarsValue) * weightFactor;
                double fiber = emptyToZero(fiberValue) * weightFactor;
                double protein = emptyToZero(proteinValue) * weightFactor;
                double salt = emptyToZero(saltValue) * weightFactor;
                //Check if total too high
                double totalWeight =  (salt + protein + fiber + carbohydrates + fat)/weightFactor;

                //Add to database if valid values
                if(carbohydrates >= carboSugars && fat >= fatSaturated && totalWeight <= weight){
                    Ingredient newIngredient = new Ingredient(name, calories, fat, fatSaturated,
                            carbohydrates, carboSugars, fiber, protein, salt);
                    //Push to firebase
                    MainActivity.DATAREF = MainActivity.DATABASE.getReference().child(MainActivity.USERID);
                    MainActivity.DATAREF.child("ingredients").child(name).setValue(newIngredient);
                    clearInput();
                    openDialog("Add Success");
                }
                //Inform of fat and carbo values
                else{
                    openDialog("Wrong Data");
                }
        }
        }
    }

    public void onClickGetDefaults(View view){
        tempIngredientList = new ArrayList<>();
        //Kurkku
        tempIngredientList.add(new Ingredient("Kurkku",11,0.1,0.1,
                1.4,1.4,0.7,0.7,0));
        //Persilja
        tempIngredientList.add(new Ingredient("Persilja",27,0.2,0.1,
                1.1,0.8,8,1.4,0));
        //Jäävuorisalaatti
        tempIngredientList.add(new Ingredient("Jäävuorisalaatti",13,0.1,0.1,
                1,1,1,1.1,0));
        //Kinkkusuikale
        tempIngredientList.add(new Ingredient("Kinkkusuikale",116,4,1.5,
                5.3,0.9,0,18,2.2));
        //Miniluumutomaatti
        tempIngredientList.add(new Ingredient("Miniluumutomaatti",25,0.3,0,
                3.5,3.3,1.4,0.6,0));
        //Mozzarella (laktoositon)
        tempIngredientList.add(new Ingredient("Mozzarella (laktoositon)",250,19,12,
                1.5,1.5,0,18,0.6));
        //Add default ingredients to your own ingredients
        MainActivity.DATAREF = MainActivity.DATABASE.getReference().child(MainActivity.USERID);
        for(Ingredient ingredient: tempIngredientList){
            String name = ingredient.getName();
            MainActivity.DATAREF.child("ingredients").child(name).setValue(ingredient);
        }
        tempIngredientList.clear();
        openDialog("Defaults Imported");
    }

    private void clearInput() {
        nameField.setText("");
        weightField.setText("");
        caloriesField.setText("");
        fatField.setText("");
        fatSaturatedField.setText("");
        carbohydratesField.setText("");
        carboSugarField.setText("");
        fiberField.setText("");
        proteinField.setText("");
        saltField.setText("");
    }

    private void openDialog(String tag){
        noNameGivenDialog dialog = new noNameGivenDialog();
        dialog.show(getSupportFragmentManager(),tag);
    }

    private double emptyToZero(String value){
        if(value.matches("")){
            return 0.0;
        }
        else{
            return Double.parseDouble(value);
        }
    }
}
