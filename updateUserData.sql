alter table userdata add EquipItem int;
alter table userdata add constraint BasicItem default 0 for equipitem
alter table userdata alter column equipitem int not null

select * from userdata;

update userdata set ip ='0000', EquipItem = 0 where id = 'hello';
update userdata set EquipItem = 0;

/* 추후 아이템 테이블 만든후 item 넘버를 외래키로 지정 수정 할것