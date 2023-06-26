// mqtt_publisher.cpp
//
// This is a Paho MQTT C++ client, sample application.
// An adapted version of 
// https://github.com/eclipse/paho.mqtt.cpp/blob/master/src/samples/async_publish_time.cpp
//
// It's a fairly contrived, but useful example of an MQTT data monitor and
// publisher, using the C++ asynchronous client interface. A fairly common
// usage for MQTT applications to monitor a sensor and publish the reading
// when it changes by a "significant" amount (whatever that may be).
// This might be temperature, pressure, humidity, soil moisture, CO2 levels,
// or anything like that.
//
// Since we don't have a universal sensor to use for this example, we simply
// use time itself as out input data. We periodically "sample" the time
// value and when it changes by more than our required delta amount, we
// publish the time. In this case we use the system clock, measuring the
// time with millisecond precision.
//
// The sample demonstrates:
//  - Connecting to an MQTT server/broker
//  - Sampling a value
//  - Publishing messages using a `topic` object
//  - Last will and testament
//  - Callbacks with lambdas
//  - Using `create_options`
//  - Creating options with builder classes
//  - Offline buffering in the client
//

#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>	// For sleep
#include <atomic>
#include <chrono>
#include <cstring>
#include <boost/asio/ip/host_name.hpp>
#include <sys/types.h>
#include <unistd.h>
#include "mqtt/async_client.h"

using namespace std;
using namespace std::chrono;

const std::string DFLT_SERVER_ADDRESS { "tcp://localhost:1883" };

// The QoS for sending data
const int QOS = 1;

// How often to sample the "data"
const auto SAMPLE_PERIOD = milliseconds(5);

// How much the "data" needs to change before we publish a new value.
const int DELTA_MS = 100;

// How many to buffer while off-line
const int MAX_BUFFERED_MESSAGES = 1200;

// --------------------------------------------------------------------------
// Gets the current time as the number of milliseconds since the epoch:
// like a time_t with ms resolution.

uint64_t timestamp()
{
	auto now = system_clock::now();
	auto tse = now.time_since_epoch();
	auto msTm = duration_cast<milliseconds>(tse);
	return uint64_t(msTm.count());
}

// --------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	//The hostname and process id to generate the client name
	const std::string client_name = boost::asio::ip::host_name() + "_" + std::to_string(getpid());
	//const string client_name = std::to_string(getpid());
	cout << "Client name:" << client_name << "\n";

	// The server URI (address)
	string address = (argc > 1) ? string(argv[1]) : DFLT_SERVER_ADDRESS;

	// The amount of time to run (in ms). Zero means "run forever".
	uint64_t trun = (argc > 2) ? stoll(argv[2]) : 0LL;

	cout << "Initializing for server '" << address << "'..." << endl;

	// We configure to allow publishing to the client while off-line,
	// and that it's OK to do so before the 1st successful connection.
	auto createOpts = mqtt::create_options_builder()
						  .send_while_disconnected(true, true)
					      .max_buffered_messages(MAX_BUFFERED_MESSAGES)
						  .delete_oldest_messages()
						  .finalize();

	mqtt::async_client cli(address, "", createOpts);

	// Set callbacks for when connected and connection lost.

	cli.set_connected_handler([&cli](const std::string&) {
		std::cout << "*** Connected ("
			<< timestamp() << ") ***" << std::endl;
	});

	cli.set_connection_lost_handler([&cli](const std::string&) {
		std::cout << "*** Connection Lost ("
			<< timestamp() << ") ***" << std::endl;
	});

	auto willMsg = mqtt::message("test/events", "Time publisher disconnected", 1, true);
	auto connOpts = mqtt::connect_options_builder()
		.clean_session()
		.will(willMsg)
		.automatic_reconnect(seconds(1), seconds(10))
		.finalize();

	try {
		// Note that we start the connection, but don't wait for completion.
		// We configured to allow publishing before a successful connection.
		cout << "Starting connection..." << endl;
		cli.connect(connOpts);

		auto top = mqtt::topic(cli, "data/" + client_name + "/time", QOS);
		cout << "Publishing data..." << endl;

		while (timestamp() % DELTA_MS != 0)
			;

		uint64_t	t = timestamp(),
					tlast = t,
					tstart = t;

		top.publish(to_string(t));

		while (true) {
			this_thread::sleep_for(SAMPLE_PERIOD);

			t = timestamp();
			//cout << t << endl;
			if (abs(int(t - tlast)) >= DELTA_MS)
				top.publish(to_string(tlast = t));

			if (trun > 0 && t >= (trun + tstart))
				break;
		}

		// Disconnect
		cout << "\nDisconnecting..." << endl;
		cli.disconnect()->wait();
		cout << "  ...OK" << endl;
	}
	catch (const mqtt::exception& exc) {
		cerr << exc.what() << endl;
		return 1;
	}

 	return 0;
}

