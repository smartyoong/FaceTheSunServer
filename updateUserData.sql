alter table userdata add EquipItem int;
alter table userdata add constraint BasicItem default 0 for equipitem
alter table userdata alter column equipitem int not null

select * from userdata;

update userdata set ip ='0000', EquipItem = 0 where id = 'hello';
update userdata set EquipItem = 0;

/* ���� ������ ���̺� ������ item �ѹ��� �ܷ�Ű�� ���� ���� �Ұ�