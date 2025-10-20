package com.parissakalaee.sensorIoT.data

data class SensorReading(
    val id: String,
    val name: String,
    val value: Double,
    val unit: String,
    val timestamp: Long = System.currentTimeMillis(),
    val isConnected: Boolean = true
)
