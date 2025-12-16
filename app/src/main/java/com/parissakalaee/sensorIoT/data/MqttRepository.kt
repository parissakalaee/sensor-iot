package com.parissakalaee.sensorIoT.data

import android.content.Context
import kotlinx.coroutines.channels.awaitClose
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.callbackFlow
import org.eclipse.paho.client.mqttv3.*
import org.json.JSONObject

class MqttRepository(private val context: Context) {

    private val brokerUrl = "tcp://test.mosquitto.org:1883"
    private val topic = "parissa/sensors"
    private var mqttClient: MqttClient? = null

    fun observeSensorData(): Flow<SensorReading> = callbackFlow {
        val clientId = "AndroidClient-${System.currentTimeMillis()}"
        mqttClient = MqttClient(brokerUrl, clientId, null)

        val options = MqttConnectOptions().apply {
            isCleanSession = true
            connectionTimeout = 10
            keepAliveInterval = 60
        }

        mqttClient?.setCallback(object : MqttCallback {
            override fun connectionLost(cause: Throwable?) {
                println("MQTT connection lost: ${cause?.message}")
            }

            override fun messageArrived(topic: String?, message: MqttMessage?) {
                message?.let {
                    val json = JSONObject(String(it.payload))
                    val temp = json.getDouble("temperature")
                    val humid = json.getDouble("humidity")

                    trySend(SensorReading("Temp_XD", "Temperature", temp, "°C"))
                    trySend(SensorReading("Humid_FR", "Humidity", humid, "%"))
                }
            }

            override fun deliveryComplete(token: IMqttDeliveryToken?) {}
        })

        mqttClient?.connect(options)
        mqttClient?.subscribe(topic)
        println("MQTT connected and subscribed to $topic")

        awaitClose {
            mqttClient?.disconnect()
            mqttClient?.close()
        }
    }
}