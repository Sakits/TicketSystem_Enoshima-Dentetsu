架构一.一个集合存已登录用户的username和privilege（为了方便第一个SF操作：query profile），一个集合只存已登录用户的username（精简版方便查找用户是否登录，由于login和logout都只是N所以影响不大），一个集合现有存在用户的所有用户相关信息，一个集合存username-订单的map
缺点是1.(F)modify profile的时候如果改了privilege，需要同时在已登录用户里更改privilege
     2.(SF)query profile的时候password成为冗余信息
架构二.同上，但是额外做一个username-password的map,使得在所有其它信息中都不出现password
缺点是1.(F)login时要做额外的一次password验证查找