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

public class saladInfo extends AppCompatActivity {
    public Salad CURRENT_SALAD;
    AnyChartView anyChart;
    TextView topTitle;
    TextView energyInfo;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_salad_info);
        CURRENT_SALAD = browseSalads.SELECTED_SALAD;

        anyChart = findViewById(R.id.any_chart_view);
        topTitle = findViewById(R.id.textViewTopTitle);
        energyInfo = findViewById(R.id.textViewEnergy);

        Ingredient ingredient1 = CURRENT_SALAD.getIngredient1();
        Ingredient ingredient2 = CURRENT_SALAD.getIngredient2();
        Ingredient ingredient3 = CURRENT_SALAD.getIngredient3();

        String title = ingredient1.getName() + "\n" + ingredient2.getName() + "\n" + ingredient3.getName();

        //Top title
        topTitle.setText(title);

        //Chart
        Cartesian Bar =  AnyChart.column();

        List<DataEntry> data = new ArrayList<>();

        double fatSaturated = ingredient1.getSaturatedFat() + ingredient2.getSaturatedFat()
                + ingredient3.getSaturatedFat();
        double fatNonSaturated = ingredient1.getFat() + ingredient2.getFat()
                + ingredient3.getFat() - fatSaturated;
        double carbosugars = ingredient1.getCarbohydratesSugar()
                + ingredient2.getCarbohydratesSugar() + ingredient3.getCarbohydratesSugar();
        double carbohydratesOther = ingredient1.getCarbohydrates() + ingredient2.getCarbohydrates()
                + ingredient3.getCarbohydrates()- carbosugars;
        double dietaryFiber = ingredient1.getDietaryFiber() + ingredient2.getDietaryFiber()
                + ingredient3.getDietaryFiber();
        double protein = ingredient1.getProtein() + ingredient2.getProtein()
                + ingredient3.getProtein();
        double salt = ingredient1.getSalt() + ingredient2.getSalt() + ingredient3.getSalt();

        String titleCarboSugars = getResources().getString(R.string.title_carbo_sugars_chart);
        String titleCarboOther = getResources().getString(R.string.title_carbo_other_chart);
        String titleOtherFat = getResources().getString(R.string.title_fat_chart);
        String titleSaturatedFat = getResources().getString(R.string.title_fat_saturated_chart);
        String titleDietaryFiber = getResources().getString(R.string.title_fiber_chart);
        String titleProtein = getResources().getString(R.string.title_protein_chart);
        String titleSalt = getResources().getString(R.string.title_salt_chart);
        String titleChart = getResources().getString(R.string.title_salad_nutriens_chart);

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

        double caloriesDouble = ingredient1.getCalories() + ingredient2.getCalories() + ingredient3.getCalories();
        String calories = String.format("%.1f",caloriesDouble);
        energyInfo.setText(getResources().getString(R.string.title_energy_chart) + calories + " kcal");
    }
}
