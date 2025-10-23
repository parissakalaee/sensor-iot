package com.parissakalaee.sensorIoT.viewmodels

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.parissakalaee.sensorIoT.data.SensorReading
import com.parissakalaee.sensorIoT.data.SensorRepository
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch

class DashboardViewModel : ViewModel() {

    init {
        startSimulatingTemperature()
    }

    private val repository = SensorRepository()
    private val _sensors = MutableStateFlow(repository.getSensors())
    val sensors: StateFlow<List<SensorReading>> = _sensors

    private fun startSimulatingTemperature() {
        var i = 0.0
        viewModelScope.launch {
            while (true) {
                delay(2000)
                i = i + 1.0
                _sensors.value =  _sensors.value.map {
                    if(it.id == "Temp_XD"){
                        it.copy(
                            value = i,
                            timestamp = System.currentTimeMillis()
                        )
                    }else{
                        it
                    }
                }
            }
        }
    }
}

