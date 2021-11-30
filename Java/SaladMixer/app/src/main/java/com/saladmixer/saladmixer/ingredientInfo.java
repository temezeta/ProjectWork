package com.saladmixer.saladmixer;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.anychart.AnyChart;
import com.anychart.AnyChartView;
import com.anychart.chart.common.dataentry.DataEntry;
import com.anychart.chart.common.dataentry.ValueDataEntry;
import com.anychart.charts.Cartesian;

import java.util.ArrayList;
import java.util.List;

public class ingredientInfo extends AppCompatActivity {
    public Ingredient CURRENT_INGREDIENT;
    AnyChartView anyChart;
    TextView topTitle;
    TextView energyInfo;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ingredient_info);
        CURRENT_INGREDIENT = BrowseIngredients.SELECTED_INGREDIENT;

        anyChart = findViewById(R.id.any_chart_view);
        topTitle = findViewById(R.id.textViewTopTitle);
        energyInfo = findViewById(R.id.textViewEnergy);

        //Top title
        topTitle.setText(CURRENT_INGREDIENT.getName());

        //Chart
        Cartesian Bar =  AnyChart.column();

        List<DataEntry> data = new ArrayList<>();
        double fatSaturated = CURRENT_INGREDIENT.getSaturatedFat();
        double fatNonSaturated = CURRENT_INGREDIENT.getFat()- fatSaturated;
        double carbosugars = CURRENT_INGREDIENT.getCarbohydratesSugar();
        double carbohydratesOther = CURRENT_INGREDIENT.getCarbohydrates() - carbosugars;
        double dietaryFiber = CURRENT_INGREDIENT.getDietaryFiber();
        double protein = CURRENT_INGREDIENT.getProtein();
        double salt = CURRENT_INGREDIENT.getSalt();

        String titleCarboSugars = getResources().getString(R.string.title_carbo_sugars_chart);
        String titleCarboOther = getResources().getString(R.string.title_carbo_other_chart);
        String titleOtherFat = getResources().getString(R.string.title_fat_chart);
        String titleSaturatedFat = getResources().getString(R.string.title_fat_saturated_chart);
        String titleDietaryFiber = getResources().getString(R.string.title_fiber_chart);
        String titleProtein = getResources().getString(R.string.title_protein_chart);
        String titleSalt = getResources().getString(R.string.title_salt_chart);
        String titleChart = getResources().getString(R.string.title_nutrients_chart);

        if(fatSaturated != 0){
            data.add(new ValueDataEntry(titleSaturatedFat, fatSaturated));
        }
        if(fatNonSaturated != 0){
            data.add(new ValueDataEntry(titleOtherFat, fatNonSaturated));
        }
        if(carbosugars != 0){
            data.add(new ValueDataEntry(titleCarboSugars, carbosugars));
        }
        if(carbohydratesOther != 0){
            data.add(new ValueDataEntry(titleCarboOther, carbohydratesOther));
        }
        if(dietaryFiber != 0){
            data.add(new ValueDataEntry(titleDietaryFiber, dietaryFiber));
        }
        if(protein != 0){
            data.add(new ValueDataEntry(titleProtein, protein));
        }
        if(salt != 0){
            data.add(new ValueDataEntry(titleSalt, salt));
        }

        Bar.data(data);
        Bar.title(titleChart);
        Bar.background().fill("#FFFFFF 1.0");
        Bar.yAxis(0).labels().format("{%Value} g");
        anyChart.setChart(Bar);

        String calories = String.format("%.1f",CURRENT_INGREDIENT.getCalories());
        energyInfo.setText(getResources().getString(R.string.title_energy_chart) + calories + " kcal");
    }
}
