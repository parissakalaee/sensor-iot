package com.parissakalaee.sensorIoT.ui.components

import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.CheckCircle
import androidx.compose.material.icons.filled.Warning
import androidx.compose.material3.Card
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.parissakalaee.sensorIoT.data.SensorReading
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale


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
            Text(
                text = sensor.name,
                style = MaterialTheme.typography.titleLarge
            )
            Spacer(modifier = Modifier.height(8.dp))
            Text(
                text = "%.1f %s".format(sensor.value, sensor.unit),
                style = MaterialTheme.typography.headlineMedium
            )
            Spacer(modifier = Modifier.height(8.dp))

            Row (verticalAlignment = Alignment.CenterVertically){
                Icon(
                    imageVector = if (sensor.isConnected) {
                        Icons.Filled.CheckCircle
                    } else {
                        Icons.Filled.Warning
                    },
                    tint = if (sensor.isConnected) {
                        Color(0xFF4CAF50)
                    } else {
                        Color.Red
                    },
                    contentDescription = "Connection Status",
                    modifier = Modifier.size(24.dp)
                )
                Spacer(modifier = Modifier.width(8.dp))
                Text(
                    text = if (sensor.isConnected) {
                        "Connected"
                    } else {
                        "Disconnected"
                    },
                )}
            Spacer(modifier = Modifier.height(8.dp))
            Text(
                text = formatTimestamp(sensor.timestamp)
            )
        }
    }
}

private fun formatTimestamp(timestamp: Long): String {
    val formatter = SimpleDateFormat("HH:mm:ss", Locale.getDefault())
    return formatter.format(Date(timestamp))
}

@Preview(showBackground = true)
@Composable
fun SensorCardPreview() {
    SensorCard(
        sensor = SensorReading("test", "Temperature", 22.5, "°C")
    )
}