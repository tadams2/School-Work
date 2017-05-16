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

hash_fields = FOREACH part GENERATE
	    FLATTEN(hashtags) as tags,
	    id;



hash_groups = GROUP hash_fields BY tags;

top100	 = FOREACH hash_groups GENERATE    
	   FLATTEN(hash_fields.tags) AS tag,
	   COUNT(hash_fields.id) AS cnt;

hash_dist = DISTINCT top100;

hash_order = ORDER hash_dist BY cnt  DESC;
lmt_hash = limit hash_order 100;

tag_id   = FOREACH hash_groups GENERATE
	 FLATTEN(hash_fields.id) AS ID,
	 FLATTEN(hash_fields.tags) AS tag;

dist_tag_id = DISTINCT tag_id;

join_list = JOIN dist_tag_id BY tag, top100 BY tag;

dist_join_list = DISTINCT join_list;

final_list = FOREACH dist_join_list GENERATE
	   ID,
	   dist_tag_id::tag,
	   cnt;	   

final_list_order = ORDER final_list BY cnt DESC;


STORE final_list_order INTO '$output';
