#pragma once

void log(String msg);
void log(char* msg);

// template<typename Arg>
// String concat_multitype(String o, Arg&& arg) {
//   return o + String(std::forward<Arg>(arg));
// }

// template<typename Arg, typename ...Args>
// String concat_multitype(String& o, Arg&& arg, Args&&... args)
// {
//   o = o + String(std::forward<Arg>(arg));
//   return concat_multitype(o, std::forward<Args>(args)...);
// }

// template<typename ... Args>
// String merge_s(Args... args)
// {
//   String msg;
//   concat_multitype(msg, args...);
//   return msg;
// }

// template<typename ... Args>
// char* merge_c(Args ... args)
// {
//   String msg;
//   concat_multitype(msg, args...);
//   char *ch_msg = new char[sizeof(msg.c_str())];
//   strcpy(ch_msg, msg.c_str());
//   return ch_msg;
// }

// template<typename ... Args>
// void log(Args ... args)
// {
//   String msg = merge_s(args...);

//   if (Serial)
//     Serial.println(msg);

//   if (mqttClient.connected())
//     mqttClient.publish(SYSTEM_NAME SAT_NAME "/events", msg.c_str());
// }
