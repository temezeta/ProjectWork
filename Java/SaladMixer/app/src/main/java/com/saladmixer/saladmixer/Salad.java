package com.saladmixer.saladmixer;

import java.util.ArrayList;

public class Salad {
    private String name;
    private Ingredient ingredient1;
    private Ingredient ingredient2;
    private Ingredient ingredient3;

    Salad(){}

    public Salad(String name, Ingredient ingredient_1, Ingredient ingredient_2, Ingredient ingredient_3){
        this.name = name;
        this.ingredient1 = ingredient_1;
        this.ingredient2 = ingredient_2;
        this.ingredient3 = ingredient_3;
    }

    public String getName(){
        return name;
    }

    public Ingredient getIngredient1() {
        return ingredient1;
    }


    public Ingredient getIngredient2() {
        return ingredient2;
    }


    public Ingredient getIngredient3() {
        return ingredient3;
    }
}
