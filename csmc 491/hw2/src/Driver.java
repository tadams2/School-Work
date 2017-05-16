package hw2;

import java.util.Arrays;
import java.util.Properties;

import org.apache.kafka.clients.consumer.KafkaConsumer;
import org.apache.kafka.clients.producer.KafkaProducer;

public class Driver {
public static void main(String[] args){
		
		Properties prop = new Properties();
		prop.put("request.required.acks", "1");
		prop.put("bootstrap.servers", "0:9092");
        prop.put("metadata.broker.list", "localhost:9092");
        prop.put("key.deserializer", "org.apache.kafka.common.serialization.StringDeserializer");
        prop.put("value.deserializer", "org.apache.kafka.common.serialization.ByteArrayDeserializer");
        prop.put("key.serializer", "org.apache.kafka.common.serialization.StringSerializer");
        prop.put("value.serializer", "org.apache.kafka.common.serialization.ByteArraySerializer");
		KafkaProducer<String, byte[]> kafkaProducer = new KafkaProducer<String, byte[]>(prop);
		Producer producer = new Producer();

		Properties props = new Properties();
        props.put("acks", "all");
        props.put("retries", 0);
        props.put("batch.size", 16384);
        props.put("linger.ms", 1);
        props.put("buffer.memory", 33554432);
        props.put("bootstrap.servers", "localhost:9092");
        props.put("metadata.broker.list", "localhost:9092");
        props.put("group.id", "test");
        props.put("enable.auto.commit", "true");
        props.put("auto.commit.interval.ms", "1000");
        props.put("session.timeout.ms", "30000");
        props.put("key.deserializer", "org.apache.kafka.common.serialization.StringDeserializer");
        props.put("value.deserializer", "org.apache.kafka.common.serialization.ByteArrayDeserializer");
        props.put("key.serializer", "org.apache.kafka.common.serialization.StringSerializer");
        props.put("value.serializer", "org.apache.kafka.common.serialization.ByteArraySerializer");
		KafkaConsumer<String, byte[]> kafkaConsumer = new KafkaConsumer<String, byte[]>(props);
    	Consumer consumer = new Consumer();
    	
		try
		{
			String topic = args[0];
        	String numMessages = args[1];
        	String numSeconds = args[2];
        	
        	kafkaConsumer.subscribe(Arrays.asList(topic));
			producer.KafkaTweetProducer(kafkaProducer, topic);
			consumer.consume(kafkaConsumer, numMessages, numSeconds);

		}
		catch(Exception e)
		{
			System.out.println(e.toString());
			System.out.println("The useage of this file is java consumer.java <topic> <numMessagePerFile> <numSecondsPerFile>");

		}

	}
	
}
