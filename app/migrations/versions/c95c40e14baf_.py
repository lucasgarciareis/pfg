"""empty message

Revision ID: c95c40e14baf
Revises: 
Create Date: 2021-04-08 21:20:47.593398

"""
from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision = 'c95c40e14baf'
down_revision = None
branch_labels = None
depends_on = None


def upgrade():
    # ### commands auto generated by Alembic - please adjust! ###
    op.create_table('humidity',
    sa.Column('id', sa.Integer(), nullable=False),
    sa.Column('humidity', sa.Integer(), nullable=True),
    sa.Column('date', sa.Numeric(precision=17, scale=7, asdecimal=False), nullable=True),
    sa.PrimaryKeyConstraint('id')
    )
    op.create_table('light',
    sa.Column('id', sa.Integer(), nullable=False),
    sa.Column('light', sa.Integer(), nullable=True),
    sa.Column('date', sa.Numeric(precision=17, scale=7, asdecimal=False), nullable=True),
    sa.PrimaryKeyConstraint('id')
    )
    op.create_table('movement',
    sa.Column('id', sa.Integer(), nullable=False),
    sa.Column('movement', sa.Boolean(), nullable=True),
    sa.Column('date', sa.Numeric(precision=17, scale=7, asdecimal=False), nullable=True),
    sa.PrimaryKeyConstraint('id')
    )
    op.create_table('pressure',
    sa.Column('id', sa.Integer(), nullable=False),
    sa.Column('pressure', sa.Float(), nullable=True),
    sa.Column('date', sa.Numeric(precision=17, scale=7, asdecimal=False), nullable=True),
    sa.PrimaryKeyConstraint('id')
    )
    op.create_table('sound',
    sa.Column('id', sa.Integer(), nullable=False),
    sa.Column('sound', sa.Float(), nullable=True),
    sa.Column('date', sa.Numeric(precision=17, scale=7, asdecimal=False), nullable=True),
    sa.PrimaryKeyConstraint('id')
    )
    op.create_table('temperature',
    sa.Column('id', sa.Integer(), nullable=False),
    sa.Column('temperature', sa.Float(), nullable=True),
    sa.Column('date', sa.Numeric(precision=17, scale=7, asdecimal=False), nullable=True),
    sa.PrimaryKeyConstraint('id')
    )
    op.create_table('vibration',
    sa.Column('id', sa.Integer(), nullable=False),
    sa.Column('vibration', sa.Boolean(), nullable=True),
    sa.Column('date', sa.Numeric(precision=17, scale=7, asdecimal=False), nullable=True),
    sa.PrimaryKeyConstraint('id')
    )
    # ### end Alembic commands ###


def downgrade():
    # ### commands auto generated by Alembic - please adjust! ###
    op.drop_table('vibration')
    op.drop_table('temperature')
    op.drop_table('sound')
    op.drop_table('pressure')
    op.drop_table('movement')
    op.drop_table('light')
    op.drop_table('humidity')
    # ### end Alembic commands ###
