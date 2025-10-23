package com.parissakalaee.sensorIoT.viewmodels

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.parissakalaee.sensorIoT.data.SensorReading
import com.parissakalaee.sensorIoT.data.SensorRepository
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch
import kotlin.random.Random

class DashboardViewModel : ViewModel() {

    init {
        startSimulatingValues()
    }

    private val repository = SensorRepository()
    private val _sensors = MutableStateFlow(repository.getSensors())
    val sensors: StateFlow<List<SensorReading>> = _sensors

    private fun startSimulatingValues() {
        viewModelScope.launch {
            while (true) {
                delay(2000)
                _sensors.value =  _sensors.value.map {
                    when(it.id){
                        "Temp_XD" ->{
                            it.copy(
                                value = Random.nextDouble(15.0, 30.0),
                                timestamp = System.currentTimeMillis()
                            )
                        }
                        "Humid_FR"->{
                            it.copy(
                                value = Random.nextDouble(30.0, 80.0),
                                timestamp = System.currentTimeMillis()
                            )
                        }
                        "Pres_uY"->{
                            it.copy(
                                value = Random.nextDouble(980.0, 1020.0),
                                timestamp = System.currentTimeMillis()
                            )
                        }
                        else -> it
                    }
                }
            }
        }
    }
}

