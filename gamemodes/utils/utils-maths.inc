stock GetTickCountDifference(newtick, oldtick) {
	if (oldtick < 0 && newtick >= 0)
		return newtick - oldtick;

	else if (oldtick >= 0 && newtick < 0 || oldtick > newtick)
		return (cellmax - oldtick + 1) - (cellmin - newtick);

	return newtick - oldtick;
}


stock bool:RandomChance(chance_val) {
	new rnd_chance = random(100) + 1;
	return (chance_val >= rnd_chance) ?  true : false;
}

stock Float:Distance(Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2) {
	return VectorSize(x1 - x2, y1 - y2, z1 - z2);
}

stock Float:PointAngle(playerid, Float:xa, Float:ya, Float:xb, Float:yb) {
	new Float:carangle;
	new Float:xc, Float:yc;
	new Float:angle;
	xc = floatabs(floatsub(xa,xb));
	yc = floatabs(floatsub(ya,yb));
	if (yc == 0.0 || xc == 0.0)
	{
		if(yc == 0 && xc > 0) angle = 0.0;
		else if(yc == 0 && xc < 0) angle = 180.0;
		else if(yc > 0 && xc == 0) angle = 90.0;
		else if(yc < 0 && xc == 0) angle = 270.0;
		else if(yc == 0 && xc == 0) angle = 0.0;
	}
	else
	{
		angle = atan(xc/yc);
		if(xb > xa && yb <= ya) angle += 90.0;
		else if(xb <= xa && yb < ya) angle = floatsub(90.0, angle);
		else if(xb < xa && yb >= ya) angle -= 90.0;
		else if(xb >= xa && yb > ya) angle = floatsub(270.0, angle);
	}
	GetVehicleZAngle(GetPlayerVehicleID(playerid), carangle);
	return floatadd(angle, -carangle);
}
