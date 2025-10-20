package com.parissakalaee.sensorIoT.ui.screens

import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.tooling.preview.Preview
import androidx.lifecycle.viewmodel.compose.viewModel
import com.parissakalaee.sensorIoT.data.SensorReading
import com.parissakalaee.sensorIoT.ui.components.SensorCard
import com.parissakalaee.sensorIoT.viewmodels.DashboardViewModel

@Composable
fun DashboardScreen(viewModel: DashboardViewModel = viewModel()) {
    val sensors by viewModel.sensors.collectAsState()

    LazyColumn {
        items(sensors) { sensor ->
            SensorCard( sensor = sensor )
        }
    }
}


@Preview(showBackground = true)
@Composable
fun DashboardScreenPreview() {
    DashboardScreen()
}

@Preview(showBackground = true)
@Composable
fun SensorCardPreview() {
    SensorCard(
        sensor = SensorReading("test", "Temperature", 22.5, "°C")
    )
}