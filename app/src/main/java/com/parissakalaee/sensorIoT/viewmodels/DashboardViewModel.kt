package com.parissakalaee.sensorIoT.viewmodels

import androidx.lifecycle.ViewModel
import com.parissakalaee.sensorIoT.data.SensorReading
import com.parissakalaee.sensorIoT.data.SensorRepository
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow

class DashboardViewModel : ViewModel() {

    private val repository = SensorRepository()
    private val _sensors = MutableStateFlow(repository.getSensors())
    val sensors: StateFlow<List<SensorReading>> = _sensors
}