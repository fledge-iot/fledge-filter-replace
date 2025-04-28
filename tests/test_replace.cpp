#include <gtest/gtest.h>
#include <plugin_api.h>
#include <config_category.h>
#include <filter_plugin.h>
#include <filter.h>
#include <string.h>
#include <string>
#include <rapidjson/document.h>
#include <reading.h>
#include <reading_set.h>

using namespace std;
using namespace rapidjson;

extern "C" {
	PLUGIN_INFORMATION *plugin_info();
	void plugin_ingest(void *handle,
                   READINGSET *readingSet);
	PLUGIN_HANDLE plugin_init(ConfigCategory* config,
			  OUTPUT_HANDLE *outHandle,
			  OUTPUT_STREAM output);
	void plugin_shutdown(PLUGIN_HANDLE handle);
	int called = 0;

	void Handler(void *handle, READINGSET *readings)
	{
		called++;
		*(READINGSET **)handle = readings;
	}
};

TEST(REPLACE, ReplaceDisabled)
{
    PLUGIN_INFORMATION *info = plugin_info();
	ConfigCategory *config = new ConfigCategory("replace", info->config);
    ASSERT_NE(config, (ConfigCategory *)NULL);
	config->setItemsValueFromDefault();
    ASSERT_EQ(config->itemExists("replace"), true);
    ASSERT_EQ(config->itemExists("replacement"), true);
    ASSERT_EQ(config->itemExists("enable"), true);
    config->setValue("replace", "t");
    config->setValue("replacement", "T");
    config->setValue("enable", "false");
    ReadingSet *outReadings;
	void *handle = plugin_init(config, &outReadings, Handler);
	vector<Reading *> *readings = new vector<Reading *>;

    long testValue = 2;
	DatapointValue dpv(testValue);
	Datapoint *value = new Datapoint("test", dpv);
	Reading *in = new Reading("test", value);
	readings->push_back(in);

	ReadingSet* readingSet = new ReadingSet(readings);
	delete readings;
	plugin_ingest(handle, (READINGSET *)readingSet);

    vector<Reading *>results = outReadings->getAllReadings();
    ASSERT_EQ(results.size(), 1);
    Reading *out = results[0];
	ASSERT_STREQ(out->getAssetName().c_str(), "test");
	ASSERT_EQ(out->getDatapointCount(), 1);

    vector<Datapoint *> points = out->getReadingData();
    ASSERT_EQ(points.size(), 1);
	Datapoint *outdp = points[0];
	ASSERT_STREQ(outdp->getName().c_str(), "test");

	delete outReadings;
	delete config;
	plugin_shutdown(handle);
}

TEST(REPLACE, ReplaceAssetAndDataPointName)
{
    PLUGIN_INFORMATION *info = plugin_info();
	ConfigCategory *config = new ConfigCategory("replace", info->config);
    ASSERT_NE(config, (ConfigCategory *)NULL);
	config->setItemsValueFromDefault();
    ASSERT_EQ(config->itemExists("replace"), true);
    ASSERT_EQ(config->itemExists("replacement"), true);
    ASSERT_EQ(config->itemExists("enable"), true);
    config->setValue("replace", "t");
    config->setValue("replacement", "T");
    config->setValue("enable", "true");
    ReadingSet *outReadings;
	void *handle = plugin_init(config, &outReadings, Handler);
	vector<Reading *> *readings = new vector<Reading *>;

    long testValue = 2;
	DatapointValue dpv(testValue);
	Datapoint *value = new Datapoint("test", dpv);
	Reading *in = new Reading("test", value);
	readings->push_back(in);

	ReadingSet* readingSet = new ReadingSet(readings);
	delete readings;
	plugin_ingest(handle, (READINGSET *)readingSet);

    vector<Reading *>results = outReadings->getAllReadings();
    ASSERT_EQ(results.size(), 1);
    Reading *out = results[0];
	ASSERT_STREQ(out->getAssetName().c_str(), "TesT");
	ASSERT_EQ(out->getDatapointCount(), 1);

    vector<Datapoint *> points = out->getReadingData();
    ASSERT_EQ(points.size(), 1);
	Datapoint *outdp = points[0];
	ASSERT_STREQ(outdp->getName().c_str(), "TesT");
	delete outReadings;
	delete config;
	plugin_shutdown(handle);
}


TEST(REPLACE, ReplaceMultiple)
{
    PLUGIN_INFORMATION *info = plugin_info();
	ConfigCategory *config = new ConfigCategory("replace", info->config);
    ASSERT_NE(config, (ConfigCategory *)NULL);
	config->setItemsValueFromDefault();
    ASSERT_EQ(config->itemExists("replace"), true);
    ASSERT_EQ(config->itemExists("replacement"), true);
    ASSERT_EQ(config->itemExists("enable"), true);
    config->setValue("replace", "t");
    config->setValue("replacement", "T");
    config->setValue("enable", "true");
    ReadingSet *outReadings;
	void *handle = plugin_init(config, &outReadings, Handler);
	vector<Reading *> *readings = new vector<Reading *>;

    vector<Datapoint *> datapoints;

    long testValue = 2;
	DatapointValue dpv(testValue);
    datapoints.push_back(new Datapoint("temperature", dpv));
    datapoints.push_back(new Datapoint("vibration", dpv));
    datapoints.push_back(new Datapoint("humidity", dpv));
    datapoints.push_back(new Datapoint("test", dpv));
	
	Reading *in = new Reading("test", datapoints);
	readings->push_back(in);

	ReadingSet* readingSet = new ReadingSet(readings);
	delete readings;
	plugin_ingest(handle, (READINGSET *)readingSet);

    vector<Reading *>results = outReadings->getAllReadings();
    ASSERT_EQ(results.size(), 1);
    Reading *out = results[0];
	ASSERT_STREQ(out->getAssetName().c_str(), "TesT");
	ASSERT_EQ(out->getDatapointCount(), 4);
   
    vector<Datapoint *> points = out->getReadingData();
    ASSERT_EQ(points.size(), 4);

    Datapoint *outdp = points[0];
	ASSERT_STREQ(outdp->getName().c_str(), "TemperaTure");

    outdp = points[1];
	ASSERT_STREQ(outdp->getName().c_str(), "vibraTion");

    outdp = points[2];
	ASSERT_STREQ(outdp->getName().c_str(), "humidiTy");
	
    outdp = points[3];
	ASSERT_STREQ(outdp->getName().c_str(), "TesT");
	delete outReadings;
	delete config;
	plugin_shutdown(handle);
}
