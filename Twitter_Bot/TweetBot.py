import tweepy
import tkinter 

consumer_key = 'YpYHgA3FEpi1QR4A1iv3ewfje'
consumer_secret = '0t0JIj7oaMDFK6itjzDQXfb722cBLiVybPALwPf7GRPlFqFhFs'
access_token = '1773885107889491969-BPkRYQGZpSuDYJuMFjsgL22BaBOPZb'
access_token_secret = 'uPrVNuiHwfKUqCiVrkiVhvudfrq8d5lT67Zgv0PlHn55C'

#client for V1 endpoint
auth = tweepy.OAuth1UserHandler(consumer_key, consumer_secret,
                                access_token, access_token_secret)

oldapi = tweepy.API(auth)

#global variable to store id of the last tweet
last_tweet = ""

#creates the client for V2 endpoint
def get_client():
    api = tweepy.Client(bearer_token='AAAAAAAAAAAAAAAAAAAAALlDtAEAAAAAyWqxcstfja%2BwELEKCarsw4gSbLw%3DoderHxt3CqFqulmfiAqyjB4A88R4WaLYlcoBFtf2X7vK4tdaSN',
    access_token=access_token,
    access_token_secret=access_token_secret,
    consumer_key=consumer_key,
    consumer_secret=consumer_secret)

    return api


#takes id of a post and retweets it using the retweet function
def retweet(tweet_text, api):
    ID = tweet_text.id
    api.retweet(ID, user_auth = True)

#searches the sport centers page for most recent tweets
def search_tweets():
    #accesses my developer account
    api = get_client()
    user = api.get_user(username = 'SportsCenter')
    ID = user.id_str
    #loops
    while 1:
        
        #checks the five most recent tweets and stores the most recent
        tweets = api.get_users_tweets(id = ID, max_results = 5, exclude = retweet)
        tweet_text = tweets[0]

        #if the tweet ids do not match, retweet the tweet that was just fetched
        if tweet_text.id != last_tweet:
            last_tweet = tweet_text.id
            retweet(tweet_text, api)
       


def main():

    
    search_tweets()


if __name__=="__main__": 
    main() 

#tests with my account

