/*create table UserData
(
	ID nvarchar(16) primary key,
	password nvarchar(16),
	userlevel int default 1 not null,
	currentexp int not null default 0,
	point int not null default 0,
	IP nvarchar(128)
)
GO*/

--alter table userdata drop column password
--alter table userdata add password varbinary(128)
--insert into UserData(ID,userlevel,currentexp,point,password) values('hello',1,0,0,PWDENCRYPT('1234'))

select (case when count(*)=1 then 'ok' else 'no' end) from userdata where ID = 'hello' and PWDCOMPARE('1234',password) =1