package com.parissakalaee.sensorIoT.ui.screens

import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import com.parissakalaee.sensorIoT.data.SensorReading
import com.parissakalaee.sensorIoT.ui.components.SensorCard
import com.parissakalaee.sensorIoT.viewmodels.DashboardViewModel

@Composable
fun DashboardScreen(
    viewModel: DashboardViewModel,
    modifier: Modifier = Modifier
) {
    val sensors by viewModel.sensors.collectAsState()

    LazyColumn(modifier = modifier) {
        items(sensors) { sensor ->
            SensorCard(
                sensor = sensor,
                onToggleConnection = { viewModel.toggleSensorConnection(sensor.id) }
            )
        }
    }
}

@Preview(showBackground = true)
@Composable
fun DashboardScreenPreview() {
    Column {
        SensorCard(
            sensor = SensorReading("Temp_XD", "Temperature", 23.5, "°C"),
            onToggleConnection = { }
        )
        SensorCard(
            sensor = SensorReading("Humid_FR", "Humidity", 65.0, "%"),
            onToggleConnection = { }
        )
        SensorCard(
            sensor = SensorReading("Pres_uY", "Pressure", 1013.0, "mbar", isConnected = false),
            onToggleConnection = { }
        )
    }
}