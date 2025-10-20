package com.parissakalaee.sensorIoT

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import com.parissakalaee.sensorIoT.ui.screens.DashboardScreen
import com.parissakalaee.sensorIoT.ui.theme.SensorIoTTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            SensorIoTTheme {
                DashboardScreen()
            }
        }
    }
}