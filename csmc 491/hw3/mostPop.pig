part = LOAD '$input' USING AvroStorage();
DESCRIBE part;
-- "fields": [
--        {"name": "created_at", "type": "string" },
--        {"name": "id", "type": "long" },
--        {"name": "text", "type": "string" },
--        {"name": "source", "type": "string" },
--        {"name": "retweet_count", "type": "int" },
--        {"name": "favorite_count", "type": "int" },
--        {"name": "user_id", "type": "long" },
--        {"name": "screen_name", "type": "string" },
--        {"name": "location", "type": ["null", "string" ] },
--        {"name": "description", "type": ["null", "string" ] },
--        {"name": "followers_count", "type": "int"},
--        {"name": "statuses_count", "type": "int"},
--        {"name": "geo_enabled", "type": "boolean"},
--        {"name": "lang", "type": "string"},
--        {"name": "latitude", "type": ["null", "float"] },
--        {"name": "longitude", "type": ["null", "float"] },
--        {"name": "hashtags", "type": { "type": "array", "items" : "string" } },
--        {"name": "urls", "type": { "type": "array", "items" : "string" } },
--        {"name": "mentions", "type": { "type": "array", "items" : "string" } }
--    ]



user_groups = GROUP part BY user_id;



user_groups_cnt = FOREACH user_groups{
		sorted = ORDER part BY followers_count DESC;
		top_record = LIMIT sorted 1;
		
		GENERATE group AS userID, 
			 FLATTEN(top_record.screen_name),
			 FLATTEN(top_record.followers_count) AS followers,
			 FLATTEN(top_record.statuses_count);
}



chatters = DISTINCT user_groups_cnt;

order_groups = ORDER chatters BY followers DESC;
lmt_user_groups = LIMIT order_groups 100;

STORE lmt_user_groups INTO '$output';