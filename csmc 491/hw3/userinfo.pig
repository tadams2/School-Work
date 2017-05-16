part = LOAD '$input' USING AvroStorage();
DESCRIBE part;



user_info = FOREACH part GENERATE
    user_id,
    screen_name,
    location,
    description,
    followers_count,
    geo_enabled,
    lang;

DESCRIBE user_info;

STORE user_info INTO '$output';