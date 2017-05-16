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



each_word = FOREACH part GENERATE
	  FLATTEN(TOKENIZE(text)),
	  id;

STORE each_word INTO '$output';