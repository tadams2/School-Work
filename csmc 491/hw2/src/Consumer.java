package hw2;



import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Properties;

import org.apache.avro.io.BinaryDecoder;
import org.apache.avro.io.DecoderFactory;
import org.apache.avro.specific.SpecificDatumReader;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;
import org.apache.kafka.clients.producer.KafkaProducer;

public class Consumer {
   
    public void consume(KafkaConsumer<String, byte[]> consumer, String numMessages, String numSeconds) throws IOException{
        int messagesPerFile = Integer.parseInt(numMessages);
        int secondsPerFile = Integer.parseInt(numSeconds);
        int messages = 0;
        long curTime = 0;
       

        SpecificDatumReader<Tweet> reader = new SpecificDatumReader<Tweet>(Tweet.getClassSchema());
        Calendar cal = Calendar.getInstance();
        SimpleDateFormat format1 = new SimpleDateFormat("yyyy/MM/dd/HH");
        String formatted = format1.format(cal.getTime());
        String fileName = "/user/tadams2/in/tweets/" + formatted + "/tweets-" + System.currentTimeMillis()+ ".avro";
        Configuration config = new Configuration();
		config.addResource(new Path("/opt/hadoop/etc/hadoop/core-site.xml") );
		config.addResource(new Path("/opt/hadoop/etc/hadoop/hdfs-site.xml") );
        FileSystem system = FileSystem.get(config);
        Path path = new Path(fileName);
        FSDataOutputStream writer = system.create(path);
        curTime = System.currentTimeMillis();
       
        while(true){
            ConsumerRecords<String, byte[]> records = consumer.poll(10);
            for (ConsumerRecord<String, byte[]> record : records) {
                messages++;
                if (messages > messagesPerFile || (((System.currentTimeMillis() - curTime) / 1000) > secondsPerFile)) {
                    writer.close();
                    formatted = format1.format(cal.getTime());
                    fileName = "/user/tadams2/in/tweets/" + formatted + "/tweets-" + System.currentTimeMillis() + ".avro";
                    path = new Path(fileName);
                    writer = system.create(path);
                    System.out.println("\n\n Creating new file; filename = " + fileName + "\n\n");
                    curTime = System.currentTimeMillis();
                    messages = 0;
                }
                BinaryDecoder decoder = DecoderFactory.get().binaryDecoder(record.value(), null);
                try {
                    Tweet tweet = null;
                    tweet = reader.read(null, decoder);
                    System.out.println(tweet);
                    writer.writeUTF(tweet + "\n");
                } catch (Exception e) {
                    e.printStackTrace();
                }   
            }
        }
       
    }
    public static void main (String args[])
    {
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
    	KafkaConsumer<String, byte[]> consumer = new KafkaConsumer<String, byte[]>(props);
    	
    	

        try {
        	String topic = args[0];
        	String numMessages = args[1];
        	String numSeconds = args[2];
        	
        	consumer.subscribe(Arrays.asList(topic));
			//consume(consumer, numMessages, numSeconds);

		} catch (Exception e) {
			// TODO Auto-generated catch block
			System.out.println("The useage of this file is java consumer.java <topic> <numMessagePerFile> <numSecondsPerFile>");
			//e.printStackTrace();
		}
    }
}
