from flask_sqlalchemy import SQLAlchemy
import sqlalchemy as sa
from sqlalchemy import orm
from typing import Optional
import datetime


database = SQLAlchemy()

class Device(database.Model):
    __tablename__ = 'devices'

    id: orm.Mapped[int] = orm.mapped_column(primary_key=True)
    hwid: orm.Mapped[str] = orm.mapped_column(sa.String(16), index=True, unique=True)
    key: orm.Mapped[Optional['Key']] = orm.relationship(back_populates='device')

    def __init__(self, hwid):
        super().__init__()
        self.hwid = hwid

    def __repr__(self):
        return f'<Device {self.hwid}>'

class Key(database.Model):
    __tablename__ = 'keys'

    id: orm.Mapped[int] = orm.mapped_column(primary_key=True)
    value: orm.Mapped[str] = orm.mapped_column(sa.String(32), unique=True)
    created_at: orm.Mapped[datetime.datetime] = orm.mapped_column(default=datetime.datetime.now)
    device_id: orm.Mapped[Optional[int]] = orm.mapped_column(sa.ForeignKey('devices.id', ondelete='SET NULL'), unique=True)
    device: orm.Mapped[Optional['Device']] = orm.relationship(back_populates='key')

    def __init__(self, value):
        super().__init__()
        self.value = value

    def __repr__(self):
        return f'<Key {self.value}>'