"""empty message

Revision ID: 9b900c3865ec
Revises: 3369da6798c6
Create Date: 2021-02-23 20:12:29.545538

"""
from alembic import op
import sqlalchemy as sa
from sqlalchemy.dialects import mysql

# revision identifiers, used by Alembic.
revision = '9b900c3865ec'
down_revision = '3369da6798c6'
branch_labels = None
depends_on = None


def upgrade():
    # ### commands auto generated by Alembic - please adjust! ###
    op.alter_column('sound', 'date',
               existing_type=mysql.DATETIME(),
               nullable=False)
    # ### end Alembic commands ###


def downgrade():
    # ### commands auto generated by Alembic - please adjust! ###
    op.alter_column('sound', 'date',
               existing_type=mysql.DATETIME(),
               nullable=True)
    # ### end Alembic commands ###
