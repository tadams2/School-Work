package hw2;

import twitter4j.FilterQuery;
import twitter4j.HashtagEntity;
import twitter4j.StallWarning;
import twitter4j.Status;
import twitter4j.StatusDeletionNotice;
import twitter4j.StatusListener;
import twitter4j.TwitterStream;
import twitter4j.TwitterStreamFactory;
import twitter4j.URLEntity;
import twitter4j.UserMentionEntity;
import twitter4j.conf.ConfigurationBuilder;
import org.apache.avro.generic.GenericRecord;
import org.apache.avro.io.BinaryEncoder;
import org.apache.avro.io.DatumWriter;
import org.apache.avro.io.EncoderFactory;
import org.apache.avro.specific.SpecificDatumWriter;
import org.apache.kafka.clients.producer.*;
import kafka.producer.*;
import kafka.producer.ProducerConfig;
import java.io.ByteArrayOutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

public class Producer {

	public static TwitterStream TwitterConnecter() {
		ConfigurationBuilder config = new ConfigurationBuilder();		
		config.setDebugEnabled(true).setOAuthConsumerKey("SdhIwZ2bkJhN2CoVQxvmSvw2O");
		config.setOAuthConsumerSecret("xWN6ckgD2HtlGgAYqiAnBC5Tw3lnYpQt4azWMKtTFLroXotwNe");
		config.setOAuthAccessToken("4896822581-QjPdHKdH0apGS5ItosY8nMZpyVU3Zpg5728F5Zj");
		config.setOAuthAccessTokenSecret("D4O0LIpNbTH1cbXirZYK8mfIsuuz2QHxE2eeZbhuKV7ZC");
		
		System.out.println("trying to connect");
		TwitterStream tweets = new TwitterStreamFactory(config.build()).getInstance();
		System.out.println("trying to connect");
		
		return tweets;
	}
	
	public void KafkaTweetProducer(final KafkaProducer<String, byte[]> producer, final String topic){

		TwitterStream tweets = TwitterConnecter();

		
		tweets.addListener(new StatusListener() {

			@Override
			public void onException(Exception arg0) {
				// TODO Auto-generated method stub
				//cry
			}

			@Override
			public void onDeletionNotice(StatusDeletionNotice arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void onScrubGeo(long arg0, long arg1) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void onStallWarning(StallWarning arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void onStatus(Status status) {
				// TODO Auto-generated method stub
				//System.out.println(status.getText());
				byte[] tweetToSend = kafkaEncoder(createTweet(status), producer);
				//		CHANGE TOPIC TO COMMAND LINE ARG
				ProducerRecord<String, byte[]> sendRecord = new ProducerRecord<String, byte[]>(topic, tweetToSend);
				producer.send(sendRecord);
			}

			@Override
			public void onTrackLimitationNotice(int arg0) {
				// TODO Auto-generated method stub
				
			}
			
			
			
			
		});
		FilterQuery filter = new FilterQuery();
		String[] filterStr = new String[] {"MonteCristo", "Crumbzz", "HotshotGG", "c9Sneaky", "c9Hai", "c9Jensen", "Cloud 9",
				"CLGAphromoo", "Doublelift", "League of Legends"};
		filter.track(filterStr);
		filter.language("en");
		tweets.filter(filter);
		
	}

	public static Tweet createTweet(Status status)
	{
		User user = new User();
		user.put("id", status.getUser().getId());
		user.put("screen_name", status.getUser().getScreenName());
		user.put("description", status.getUser().getDescription());
		user.put("followers_count", status.getUser().getFollowersCount());
		user.put("geo_enabled", status.getUser().isGeoEnabled());
		user.put("lang", status.getUser().getLang());
		user.put("location", status.getUser().getLocation());
		user.put("statuses_count", status.getUser().getStatusesCount());
		

		
		Tweet tweet = new Tweet();
		tweet.put("id", status.getId());
		tweet.put("created_at", status.getCreatedAt().toString());
		tweet.put("favorite_count", status.getFavoriteCount());
		tweet.put("retweet_count", status.getRetweetCount());
		tweet.put("source", status.getSource());
		tweet.put("text", status.getText());
		tweet.put("user", user);

		HashtagEntity[] tags = status.getHashtagEntities();
		List<CharSequence> hashtags = new ArrayList<CharSequence>();
		for(HashtagEntity tag : tags) {
			hashtags.add(tag.getText());
		}
		tweet.put("hashtags", hashtags);
		
		URLEntity[] urlList = status.getURLEntities();
		List<CharSequence> urls = new ArrayList<CharSequence>();
		for(URLEntity url : urlList) {
			urls.add(url.getText());
		}
		tweet.put("urls", urls);
		
		UserMentionEntity[] mentionList = status.getUserMentionEntities();
		List<CharSequence> userMentions = new ArrayList<CharSequence>();
		for(UserMentionEntity mention : mentionList) {
			userMentions.add(mention.getText());
		}
		tweet.put("user_mentions", userMentions);
		
		
		
		
		return tweet;
	}
	
	public static byte[] kafkaEncoder(Tweet tweet, KafkaProducer<String, byte[]> producer)
	{
		
		ByteArrayOutputStream outS = new ByteArrayOutputStream();
		BinaryEncoder encoder = EncoderFactory.get().binaryEncoder(outS, null);
		DatumWriter<Tweet>writer = new SpecificDatumWriter<Tweet>(Tweet.getClassSchema());

		try {
		
			writer.write(tweet, encoder);
	
			encoder.flush();
	
			outS.close();

			byte[] serializedTweet = outS.toByteArray();
			System.out.println(serializedTweet);
			return serializedTweet;
			
		} catch (Exception e) {
			// TODO Auto-generated catch block

			e.printStackTrace();
		}

		return null;
	}
	

	public static void main(String[] args){
		
		Properties prop = new Properties();
		prop.put("request.required.acks", "1");
		prop.put("bootstrap.servers", "0:9092");
        prop.put("metadata.broker.list", "localhost:9092");
		prop.put("value.serializer", "org.apache.kafka.common.serialization.ByteArraySerializer");
		prop.put("key.serializer", "org.apache.kafka.common.serialization.StringSerializer");
		KafkaProducer<String, byte[]> producer = new KafkaProducer<String, byte[]>(prop);
		try
		{
			String topic = args[0];
			//KafkaTweetProducer(producer, topic);
		}
		catch(Exception e)
		{
			System.out.println(("the usage of this program is java Producer.java <topic>"));
		}

	}



}
