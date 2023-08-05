# emg-monitor

Update the following environment variables.

- DATABASE_URL
- MODEL_HOST
- PORT 8000

Setting up the django project :

Creating virtual environment:

`python -m venv venv`

Activate virtual environment

Linux:
`source venv/bin/activate`

Windows:
`./venv/Scripts/activate`

Install dependencies
`pip install -r requirements.txt`

Apply migrations:
`python manage.py migrate`

Collect static files :
`python manage.py collectstatic`

Start the development server:
`python manage.py runserver`
