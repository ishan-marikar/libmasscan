#include <string.h>
#include <node.h>
#include <v8.h>

#include "./libmasscan.h"

extern "C" {
  #include <ctype.h>
  #include <limits.h>

  #include "masscan/src/masscan.h"
  #include "masscan/src/masscan-version.h"
  #include "masscan/src/ranges.h"
  #include "masscan/src/string_s.h"
  #include "masscan/src/logger.h"
  #include "masscan/src/proto-banner1.h"
  #include "masscan/src/templ-payloads.h"
  #include "masscan/src/templ-port.h"
  #include "masscan/src/crypto-base64.h"
  #include "masscan/src/script.h"
  #include "masscan/src/syn-cookie.h"
  #include "masscan/src/masscan-app.h"

  static uint64_t
  parseInt(const char *str) {
    uint64_t result = 0;

    while (*str && isdigit(*str & 0xFF)) {
      result = result * 10 + (*str - '0');
      str++;
    }
    return result;
  }

  static unsigned
  ARRAY(const char *rhs) {
    const char *p = strchr(rhs, '[');
    if (p == NULL)
      return 0;
    else
      p++;
    return (unsigned)parseInt(p);
  }
}

using namespace node;
using namespace v8;

void libmasscan::Config(Handle<Object> obj) {
	HandleScope scope;
	libmasscan ms;
  struct Masscan masscan[1];
  //v8::Persistent<struct Masscan> masscan[1];

  /*
   * Initialize those defaults that aren't zero
   */
  memset(masscan, 0, sizeof(*masscan));
  masscan->blackrock_rounds = 4;
  masscan->output.is_show_open = 1; /* default: show syn-ack, not rst */
  masscan->seed = get_entropy(); /* entropy for randomness */
  masscan->wait = 10; /* how long to wait for responses when done */
  masscan->max_rate = 100.0; /* max rate = hundred packets-per-second */
  masscan->nic_count = 1;
  masscan->shard.one = 1;
  masscan->shard.of = 1;
  masscan->min_packet_size = 60;
  masscan->payloads = payloads_create();
  strcpy_s(masscan->output.rotate.directory,
           sizeof(masscan->output.rotate.directory),
           ".");
  masscan->is_capture_cert = 1;

  /* Disable stdout/stderr */

//	ms.ConfigFile(obj);
	ms.ConfigIface(obj, masscan);
//	ms.ConfigIpaddr(obj);
//	ms.ConfigHwaddr(obj);
	ms.ConfigRange(obj, masscan);
//	ms.ConfigBandwidth(obj);
//	ms.ConfigBlacklist(obj);

//	ms.ConfigTargets();

//	ms.ConfigShards(obj);
//	ms.ConfigShardTotal(obj);
//	ms.ConfigThreads(obj);
}

void libmasscan::ConfigFile(Handle<Object> obj) {
	HandleScope scope;

	if (obj->Has(v8::String::NewSymbol("config"))) {
		Handle<v8::Value> value = obj->Get(String::New("config"));

	}
}

void libmasscan::ConfigIface(Handle<Object> obj, Masscan masscan[1]) {
	HandleScope scope;

	if (obj->Has(v8::String::NewSymbol("iface"))) {
		Handle<v8::Value> value = obj->Get(String::New("iface"));

    unsigned index = ARRAY(*v8::String::Utf8Value(value->ToString()));

    if (masscan->nic_count < index + 1) {
      masscan->nic_count = index + 1;
    }

    sprintf_s(masscan->nic[index].ifname,
              sizeof(masscan->nic[index].ifname), "%s",
              *v8::String::Utf8Value(value->ToString()));
	}
}

void libmasscan::ConfigIpaddr(Handle<Object> obj) {
	HandleScope scope;

	if (obj->Has(v8::String::NewSymbol("ipaddr"))) {
		Handle<v8::Value> value = obj->Get(String::New("ipaddr"));

	} else {

	}
}

void libmasscan::ConfigHwaddr(Handle<Object> obj) {
	HandleScope scope;
	libmasscan masscan;

	if (obj->Has(v8::String::NewSymbol("mac"))) {
		Handle<v8::Value> value = obj->Get(String::New("mac"));

	} else {

	}
}

void libmasscan::ConfigRange(Handle<Object> obj, Masscan masscan[1]) {
	HandleScope scope;
  struct Range range;

	if (obj->Has(v8::String::NewSymbol("range"))) {
    Handle<v8::Array> value =
      v8::Local<v8::Array>::Cast(obj->Get(String::New("range")));

    if (!value->IsArray()) {
      /* Send to exception handler callback */
    }

    for (uint32_t i = 0; i < value->Length(); ++i) {
      const Local<Value> item = value->Get(i);

      unsigned offset = 0;
      unsigned max_offset = (unsigned)
        strlen(*v8::String::Utf8Value(item->ToString()));

      range = range_parse_ipv4(*v8::String::Utf8Value(item->ToString()),
                                                      &offset, max_offset);
      if (range.end < range.begin) {
        /* Send to exception handler callback */
      }

      rangelist_add_range(&masscan->targets, range.begin, range.end);
    }
	}
}

void libmasscan::ConfigBlacklist(Handle<Object> obj) {
	HandleScope scope;

	if (obj->Has(v8::String::NewSymbol("blacklist"))) {
		Handle<v8::Value> value = obj->Get(String::New("blacklist"));

	}
}

void libmasscan::ConfigTargets(void) {
	HandleScope scope;


}

void libmasscan::ConfigShards(Handle<Object> obj) {
	HandleScope scope;

	if (obj->Has(v8::String::NewSymbol("shards"))) {
		Handle<v8::Value> value = obj->Get(String::New("shards"));

	} else {

	}
}

void libmasscan::ConfigShardTotal(Handle<Object> obj) {
	HandleScope scope;

	if (obj->Has(v8::String::NewSymbol("shardtotal"))) {
		Handle<v8::Value> value = obj->Get(String::New("shartotal"));

	} else {

	}
}

void libmasscan::ConfigCores(void) {
	HandleScope scope;
	libmasscan masscan;

}

void libmasscan::ConfigSeed(void) {
	HandleScope scope;

}
