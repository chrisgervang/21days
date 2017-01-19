heroku create
git push heroku master
heroku ps:scale web=1
heroku addons:create heroku-postgresql:hobby-dev
heroku open