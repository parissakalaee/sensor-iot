package com.parissakalaee.sensorIoT.ui.components

import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Card
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.parissakalaee.sensorIoT.data.SensorReading


@Composable
fun SensorCard (sensor: SensorReading) {
    Card (
        modifier = Modifier
            .fillMaxWidth()
            .padding(8.dp)
    ){
        Column (
            modifier = Modifier.padding(16.dp)
        ) {
            Text(sensor.name)
            Spacer(modifier = Modifier.height(8.dp))
            Text(sensor.value.toString())
            Spacer(modifier = Modifier.height(8.dp))
            Text(sensor.unit)
            Spacer(modifier = Modifier.height(8.dp))
            Text(sensor.isConnected.toString())
        }
    }
}

@Preview(showBackground = true)
@Composable
fun SensorCardPreview() {
    SensorCard(
        sensor = SensorReading("test", "Temperature", 22.5, "°C")
    )
}