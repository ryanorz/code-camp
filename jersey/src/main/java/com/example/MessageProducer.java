package com.example;

import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.apache.kafka.clients.producer.RecordMetadata;
import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.Properties;

public class MessageProducer {
    private static MessageProducer ourInstance = new MessageProducer();

    public static MessageProducer getInstance() {
        return ourInstance;
    }

    private static Logger logger = LogManager.getLogger(MessageProducer.class.getName());

    private static KafkaProducer<String, String> kafkaProducer = null;

    private MessageProducer() {
    }

    public void init(String configFile) throws IOException {
        Properties properties = new Properties();
        properties.load(new FileInputStream(configFile));
        kafkaProducer = new KafkaProducer<>(properties);
    }

    public void send(String topic, String data) {
        kafkaProducer.send(new ProducerRecord<>(topic, "", data), (RecordMetadata metadata, Exception e) -> {
            if (e != null) {
                logger.error(e.getMessage());
                e.printStackTrace();
            }
        });
    }

    public void close() {
        kafkaProducer.close();
        logger.info("kafkaProducer closed.");
    }
}
