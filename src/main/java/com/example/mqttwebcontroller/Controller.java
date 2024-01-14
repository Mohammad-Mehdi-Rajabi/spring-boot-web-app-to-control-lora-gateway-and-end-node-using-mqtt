package com.example.mqttwebcontroller;

import org.eclipse.paho.client.mqttv3.*;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@CrossOrigin("*")
public class Controller {
    @PostMapping("/api/led/{ledNumber}/{state}")
    public ResponseEntity<?> post(@PathVariable int ledNumber, @PathVariable String  state){
        String broker = "tcp://test.mosquitto.org:1883"; // Replace with your MQTT broker URL
        String clientId = "qwertyuykbcvmskjfd54fdg";
        MemoryPersistence persistence = new MemoryPersistence();

        try {
            MqttClient mqttClient = new MqttClient(broker, clientId, persistence);
            MqttConnectOptions connOpts = new MqttConnectOptions();
            connOpts.setCleanSession(true);

            System.out.println("Connecting to MQTT broker: " + broker);
            mqttClient.connect(connOpts);
            System.out.println("Connected");

            String topic = "SINARAZIPRJ";
            String message = state.toUpperCase()+ledNumber;

            System.out.println("Publishing message: " + message);
            MqttMessage mqttMessage = new MqttMessage(message.getBytes());
            mqttClient.publish(topic, mqttMessage);
            System.out.println("Message published");

            mqttClient.disconnect();
            System.out.println("Disconnected");
        } catch (MqttException e) {
            e.printStackTrace();
        }
        return ResponseEntity.ok().build();
    }
}
