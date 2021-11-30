package com.saladmixer.saladmixer;


import java.util.ArrayList;

public class Ingredient {
    private String name;
    private double calories;
    private double fat;
    private double saturatedFat;
    private double carbohydrates;
    private double carbohydratesSugar;
    private double dietaryFiber;
    private double protein;
    private double salt;

    Ingredient(){

    }

    public Ingredient(String name, double calories, double fat, double saturatedFat,
                      double carbohydrates, double carbohydratesSugar, double dietaryFiber,
                      double protein,double salt){
        this.name = name;
        this.calories = calories;
        this.fat = fat;
        this.saturatedFat = saturatedFat;
        this.carbohydrates = carbohydrates;
        this.carbohydratesSugar = carbohydratesSugar;
        this.dietaryFiber = dietaryFiber;
        this.protein = protein;
        this.salt = salt;
    }

    public String getName(){
        return name;
    }

    public double getCalories() {
        return calories;
    }

    public double getFat() {
        return fat;
    }

    public double getSaturatedFat() {
        return saturatedFat;
    }

    public double getCarbohydrates() {
        return carbohydrates;
    }

    public double getCarbohydratesSugar() {
        return carbohydratesSugar;
    }

    public double getDietaryFiber() {
        return dietaryFiber;
    }

    public double getProtein() {
        return protein;
    }

    public double getSalt() {
        return salt;
    }
}
