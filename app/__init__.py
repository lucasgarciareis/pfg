from .model import configure as config_db
from .serializer import configure as config_ma
from flask import Flask
from flask_migrate import Migrate
import os


def create_app():
    app = Flask(__name__)
    basedir = os.path.abspath(os.path.dirname(__file__))

    app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///' + \
        os.path.join(basedir, 'crud.db')
    #app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:////tmp/crud.db'
    app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

    config_db(app)
    config_ma(app)

    Migrate(app, app.db)

    from .sound import bp_sound
    app.register_blueprint(bp_sound)

    from .xdk import bp_xdk
    app.register_blueprint(bp_xdk)

    from .vibration import bp_vibration
    app.register_blueprint(bp_vibration)

    return app
